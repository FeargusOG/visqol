// Copyright 2019 Google LLC, Andrew Hines
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "xcorr_mmd.h"

#include <math.h>

#include <algorithm>
#include <complex>
#include <memory>
#include <utility>
#include <mmd-vector.h>
#include <iostream>
#include "absl/memory/memory.h"

#include "amatrix.h"
#include "fast_fourier_transform_mmd.h"

namespace Visqol {

// Assumes inputs are column vectors
int64_t XCorrMmd::CalcBestLag(const AMatrix<double>& signal_1,
                           const AMatrix<double>& signal_2) {
  const int64_t max_lag = std::max(static_cast<int64_t>(signal_1.NumRows()),
      static_cast<int64_t>(signal_2.NumRows())) - 1;

  auto pwise_fft_vec = CalcInverseFFTPwiseProd(signal_1, signal_2);
  // Build negatives corrs.
  std::vector<double> corrs{pwise_fft_vec.get_mapped_vector()->end() - max_lag, pwise_fft_vec.get_mapped_vector()->end()};
  // Build positive corrs.
  std::vector<double> positives{pwise_fft_vec.get_mapped_vector()->begin(),
                                pwise_fft_vec.get_mapped_vector()->begin() + max_lag + 1};

  // Build total corrs.
  corrs.insert(corrs.end(), positives.begin(), positives.end());
  // Find best corr and from that the best lag.
  auto best_corr = std::max_element(corrs.cbegin(), corrs.cend());
  return std::distance(corrs.cbegin(), best_corr) - max_lag;
}

mmd::MmdVector<double> XCorrMmd::CalcInverseFFTPwiseProd(
    const AMatrix<double>& signal_1, const AMatrix<double>& signal_2) {
  std::vector<double> signal_1_vec = signal_1.ToVector();
  std::vector<double> signal_2_vec = signal_2.ToVector();

  // Add zeros until they're both the same length.
  // Reserve then resize to prevent extraneous memory being allocated.
  // Resize by itself can double the vector capacity.
  const size_t biggest_vec = signal_1.NumRows() > signal_2.NumRows() ?
      signal_1.NumRows() : signal_2.NumRows();
  if (signal_1.NumRows() > signal_2.NumRows()) {
    signal_2_vec.reserve(biggest_vec);
    signal_2_vec.resize(biggest_vec, 0.0);
  } else if (signal_1.NumRows() < signal_2.NumRows()) {
    signal_1_vec.reserve(biggest_vec);
    signal_1_vec.resize(biggest_vec, 0.0);
  }

  // Calculate how many points in FFT (next ^2 elements)
  int expon;
  frexp(std::abs((int64_t)signal_1_vec.size() * 2 - 1), &expon);
  const size_t fft_points = pow(2, expon);

  // Calculate the pointwise product of the forward fft of both signals.
  auto fft_manager = absl::make_unique<FftManager>(fft_points);

  auto pwise_prod = CalcFFTPwiseProd(signal_1_vec, signal_2_vec, fft_manager,
    fft_points);

  // FOG instead of calling ToVector, can I just return the inner vector and allow it to be copied by the mmd-vector copy-ctor??
  auto i1d =  FastFourierTransformMmd::Inverse1dConjSym(fft_manager, *pwise_prod);
  auto vec = i1d->ToVector();
  delete pwise_prod;
  delete i1d;
  return vec;
}

  AMatrix<std::complex<double>>* XCorrMmd::CalcFFTPwiseProd(
    const std::vector<double> &signal_1, const std::vector<double> &signal_2,
    const std::unique_ptr<FftManager>& fft_manager, const size_t fft_points) {

  auto fftsignal_2 = FastFourierTransformMmd::Forward1d(fft_manager,
                                                signal_2,
                                                fft_points);
  std::transform(fftsignal_2->begin(), fftsignal_2->end(),
            fftsignal_2->begin(), [](decltype(*fftsignal_2->begin())& s)
            { return conj(s); });

  auto f1d = FastFourierTransformMmd::Forward1d(fft_manager, signal_1, fft_points);
  auto pwp = f1d->PointWiseProduct(fftsignal_2);
  delete f1d;
  delete fftsignal_2;
  return pwp;
}

}  // namespace Visqol
