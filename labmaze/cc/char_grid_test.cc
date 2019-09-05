// Copyright 2016-2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ============================================================================

#include "labmaze/cc/char_grid.h"

#include "gtest/gtest.h"

namespace deepmind {
namespace labmaze {
namespace {

TEST(CharGridTest, Simple) {
  CharGrid grid("abc\n12345\n");
  EXPECT_EQ(5U, grid.width());
  EXPECT_EQ(2U, grid.height());

  EXPECT_EQ('c', grid.CellAt(0, 2));
  EXPECT_EQ('3', grid.CellAt(1, 2));

  EXPECT_EQ('\0', grid.CellAt(2, 0));
  EXPECT_EQ('\0', grid.CellAt(0, 4));
}

}  // namespace
}  // namespace labmaze
}  // namespace deepmind
