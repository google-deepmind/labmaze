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

#include <algorithm>
#include <functional>

#include "logging.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace deepmind {
namespace labmaze {

CharGrid::CharGrid(std::string text)
    : raw_data_(std::move(text)),
      rows_(absl::StrSplit(raw_data_, '\n', absl::SkipEmpty())) {
  auto it = std::max_element(rows_.begin(), rows_.end(),
                             [](absl::string_view lhs, absl::string_view rhs) {
                               return lhs.size() < rhs.size();
                             });
  CHECK(it != rows_.end());
  width_ = it->size();
}

}  // namespace labmaze
}  // namespace deepmind
