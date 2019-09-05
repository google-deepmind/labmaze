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

#ifndef LABMAZE_CC_RANDOM_MAZE_H_
#define LABMAZE_CC_RANDOM_MAZE_H_

#include <random>
#include <string>

#include "absl/strings/string_view.h"
#include "labmaze/cc/algorithm.h"
#include "labmaze/cc/text_maze.h"

namespace deepmind {
namespace labmaze {

// This class generates random text mazes of a specified size. Walls in the maze
// are represented by '*'. Optionally, the generated maze can be structured into
// rooms. In this case, the number and size of the rooms can also be configured.
// The generated maze can also contain one or more "target" positions and spawn
// points, marked by configurable single-character tokens.
class RandomMaze {
 public:
  explicit RandomMaze(int height, int width,
                      int max_rooms, int room_min_size, int room_max_size,
                      int retry_count, double extra_connection_probability,
                      int max_variations, bool has_doors, bool simplify,
                      int spawns_per_room, absl::string_view spawn_token,
                      int objects_per_room, absl::string_view object_token,
                      std::mt19937_64::result_type random_seed);

  // Generates a new random maze.
  void Regenerate();

  // Returns a string representation of the latest maze generated.
  std::string EntityLayer() const;

  // Returns a string representation of the variations layer of the
  // latest maze generated.
  std::string VariationsLayer() const;

 private:
  Size maze_size_;
  SeparateRectangleParams maze_params_;
  double extra_connection_probability_;
  int max_variations_;
  bool has_doors_;
  bool simplify_;
  int spawns_per_room_;
  std::string spawn_token_;
  int objects_per_room_;
  std::string object_token_;
  std::mt19937_64 prng_;
  TextMaze maze_;
};

}  // namespace labmaze
}  // namespace deepmind

#endif  // LABMAZE_CC_RANDOM_MAZE_H_
