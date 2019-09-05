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

#ifndef LABMAZE_CC_FLOOD_FILL_H_
#define LABMAZE_CC_FLOOD_FILL_H_

#include <bitset>
#include <limits>
#include <random>
#include <vector>

#include "labmaze/cc/text_maze.h"

namespace deepmind {
namespace labmaze {
namespace internal {

// Used to store a boolean for all characters.
using CharBoolMap =
    std::bitset<std::numeric_limits<unsigned char>::max() + 1ULL>;

// Make a CharBoolMap where all characters in 'chars' are true and all others
// are false.
template <typename C>
CharBoolMap MakeCharBoolMap(const C& chars) {
  CharBoolMap result = {};
  for (const auto& c : chars) {
    result[static_cast<unsigned char>(c)] = true;
  }
  return result;
}

// Converts row and col into a flat index for use with distance.
inline int DistanceIndex(const Rectangle& area, int row, int col) {
  return row * area.size.width + col;
}

// Flood fills from 'goal' in 'distances' where distance has the value -1.
// '*distances' is updated with the distance to the goal.
// '*connected' is appended with cells connected to goal in ascending distance
// order.
// 'distances->size()' must be equal to 'area.Area()'.
bool FloodFill(Pos goal, const Rectangle& area,
               std::vector<int>* distances,
               std::vector<Pos>* connected);

}  // namespace internal

// Structure for calculating distance to goal object from any point in a maze.
class FloodFill {
 public:
  // Finds all points attached to goal.
  // 'goal' - Flood fill starts from goal.
  // 'wall_chars' are characters for the flood fill to avoid.
  FloodFill(const TextMaze& maze, TextMaze::Layer layer, Pos goal,
            const std::vector<char>& wall_chars);

  // If goal is reachable from start, returns the minimum distance between start
  // and goal. Otherwise returns -1.
  int DistanceFrom(Pos start) const;

  // If 'goal' is reachable from 'start', returns a shortest route from 'start'
  // to 'goal' including both end points. Otherwise returns an empty vector.
  // If the route from has multiple possible branches each branch has an equal
  // chance of being chosen according to the rng.
  std::vector<Pos> ShortestPathFrom(Pos start, std::mt19937_64* rng) const;

  // Calls f(i, j, distance) for all points connected to start.
  template <typename F>
  void Visit(F&& f) {
    for (const auto& p : connected_) {
      f(p.row, p.col, distances_[internal::DistanceIndex(area_, p.row, p.col)]);
    }
  }

 private:
  std::vector<int> distances_;
  std::vector<Pos> connected_;
  Rectangle area_;
};

}  // namespace labmaze
}  // namespace deepmind

#endif  // LABMAZE_CC_FLOOD_FILL_H_
