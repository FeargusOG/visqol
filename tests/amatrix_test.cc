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

#include "amatrix.h"
#include <vector>

#include "gtest/gtest.h"

namespace Visqol {
namespace {

TEST(AMatrix, PointWiseProductRef) {
  const double kVal = 2.0;  
  const size_t kRows = 2;
  const size_t kCols = 2;
  const std::vector<double> inputs = {kVal, kVal, kVal, kVal};

  // Create the original matrix.
  AMatrix<double> mat{kRows, kCols, inputs};

  // Ensure the original matrix is correct.
  EXPECT_EQ(inputs.size(), mat.NumElements());
  EXPECT_EQ(kRows, mat.NumRows());
  EXPECT_EQ(kCols, mat.NumCols());
  for (auto it = mat.cbegin(); it != mat.cend(); it++)
  {
    EXPECT_DOUBLE_EQ(kVal, *it);
  }

  // Multiply the original matrix by itself.
  auto ppw = mat.PointWiseProduct(mat);

  // Ensure the point wise product matrix is correct.
  EXPECT_EQ(inputs.size(), ppw.NumElements());
  EXPECT_EQ(kRows, ppw.NumRows());
  EXPECT_EQ(kCols, ppw.NumCols());
  for (auto it = ppw.cbegin(); it != ppw.cend(); it++)
  {
    EXPECT_DOUBLE_EQ(kVal * kVal, *it);
  }
}

TEST(AMatrix, PointWiseProductPtr) {
  const double kVal = 2.0;  
  const size_t kRows = 2;
  const size_t kCols = 2;
  const std::vector<double> inputs = {kVal, kVal, kVal, kVal};

  // Create the original matrix.
  AMatrix<double>* mat = new AMatrix<double>{kRows, kCols, inputs};

  // Ensure the original matrix is correct.
  EXPECT_EQ(inputs.size(), mat->NumElements());
  EXPECT_EQ(kRows, mat->NumRows());
  EXPECT_EQ(kCols, mat->NumCols());
  for (auto it = mat->cbegin(); it != mat->cend(); it++)
  {
    EXPECT_DOUBLE_EQ(kVal, *it);
  }

  // Multiply the original matrix by itself.
  auto ppw = mat->PointWiseProduct(mat);

  // Ensure the point wise product matrix is correct.
  EXPECT_EQ(inputs.size(), ppw->NumElements());
  EXPECT_EQ(kRows, ppw->NumRows());
  EXPECT_EQ(kCols, ppw->NumCols());
  for (auto it = ppw->cbegin(); it != ppw->cend(); it++)
  {
    EXPECT_DOUBLE_EQ(kVal * kVal, *it);
  }

  delete mat;
  delete ppw;
}

}  // namespace
}  // namespace Visqol
