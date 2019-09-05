// Copyright 2019 DeepMind Technologies Limited.
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

#ifndef LABMAZE_CC_DEFAULTS_H_
#define LABMAZE_CC_DEFAULTS_H_

namespace deepmind {
namespace labmaze {
namespace defaults {

constexpr int kMaxRooms = 4;
constexpr int kRoomMinSize = 3;
constexpr int kRoomMaxSize = 5;
constexpr int kRetryCount = 1000;
constexpr double kExtraConnectionProbability = 0.0;
constexpr int kMaxVariations = 26;
constexpr bool kHasDoors = false;
constexpr bool kSimplify = true;
constexpr int kSpawnCount = 0;
constexpr char kSpawnToken[] = "P";
constexpr int kObjectCount = 0;
constexpr char kObjectToken[] = "G";

}  // namespace defaults
}  // namespace labmaze
}  // namespace deepmind

#endif  // LABMAZE_CC_DEFAULTS_H_
