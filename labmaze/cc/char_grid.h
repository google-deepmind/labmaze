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
//
// CharGrid: A 2D view on a string of characters.

#ifndef LABMAZE_CC_CHAR_GRID_H_
#define LABMAZE_CC_CHAR_GRID_H_

#include <cstddef>
#include <string>
#include <vector>

#include "absl/strings/string_view.h"

namespace deepmind {
namespace labmaze {

// CharGrid is a thin wrapper around a string that exposes an immutable
// (row, column)-access to grid defined by a multiline string. The size
// of the grid is implied: The grid height is the number of lines, and
// the grid width is the length of the longest line.
//
// Accessing the grid out of bounds is allowed and results in a null
// character.
class CharGrid {
 public:
  // Constructs a CharGrid from the provided multi-line string.
  explicit CharGrid(std::string text);

  // Extents of the implied grid.
  std::size_t height() const { return rows_.size(); }
  std::size_t width() const { return width_; }

  // Value of the cell at row i, column j if (i, j) is within bounds,
  // or the null character value otherwise.
  char CellAt(std::size_t i, std::size_t j) const {
    return i < height() && j < rows_[i].size() ? rows_[i][j] : '\0';
  }

 private:
  std::string raw_data_;
  std::vector<absl::string_view> rows_;
  std::size_t width_;
};


}  // namespace labmaze
}  // namespace deepmind

#endif  // LABMAZE_CC_CHAR_GRID_H_
