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
  std::vector<int> inputs = {4, 2, 2, 2};
  AMatrix<int> mat(inputs);
  EXPECT_EQ(inputs.size(), mat.NumElements());

  // for (size_t i = 0; i < inputs.size(); ++i) {
  //   EXPECT_EQ(expected[i], MiscMath::NextPowTwo(inputs[i]));
  // }
}

}  // namespace
}  // namespace Visqol
