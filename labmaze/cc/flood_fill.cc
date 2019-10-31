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

#include "labmaze/cc/flood_fill.h"

#include <utility>

namespace deepmind {
namespace labmaze {
namespace internal {

bool FloodFill(const Pos goal, const Rectangle& area,
               std::vector<int>* distances, std::vector<Pos>* connected) {
  const int flat_idx = DistanceIndex(area, goal.row, goal.col);
  if (!area.InBounds(goal) || (*distances)[flat_idx] != -1) {
    return false;
  }

  std::vector<Pos> current_points, next_points;
  current_points.push_back(goal);
  int cost = 0;
  (*distances)[flat_idx] = cost;
  while (!current_points.empty()) {
    ++cost;
    for (auto pos : current_points) {
      area.VisitNeighbours(
          pos, [&area, &next_points, distances, cost](int i, int j) {
            auto& distance = (*distances)[DistanceIndex(area, i, j)];
            if (distance == -1) {
              distance = cost;
              next_points.push_back({i, j});
            }
          });
    }
    connected->insert(connected->end(), current_points.begin(),
                      current_points.end());
    current_points.clear();
    std::swap(current_points, next_points);
  }
  return true;
}

}  // namespace internal

int FloodFill::DistanceFrom(Pos pos) const {
  if (area_.InBounds(pos)) {
    int distance = distances_[pos.row * area_.size.width + pos.col];
    return distance >= 0 ? distance : -1;
  } else {
    return -1;
  }
}

FloodFill::FloodFill(const TextMaze& maze, TextMaze::Layer layer, Pos goal,
                     const std::vector<char>& wall_chars)
    : area_(maze.Area()) {
  auto is_wall = internal::MakeCharBoolMap(wall_chars);
  distances_.reserve(maze.Area().Area());
  maze.Visit(layer, [this, &is_wall](int i, int j, int c) {
    distances_.push_back(is_wall[c] ? -2 : -1);
  });
  internal::FloodFill(goal, area_, &distances_, &connected_);
}

std::vector<Pos> FloodFill::ShortestPathFrom(Pos pos,
                                             std::mt19937_64* rng) const {
  std::vector<Pos> result;

  int distance = DistanceFrom(pos);
  if (distance == -1) {
    return result;
  }
  result.reserve(distance + 1);
  result.push_back(pos);
  while (distance--) {
    const auto& next_pos = result.back();
    result.emplace_back();
    int choice = 0;
    area_.VisitNeighbours(
        next_pos, [&result, rng, &choice, distance, this](int i, int j) {
          if (distances_[internal::DistanceIndex(area_, i, j)] == distance) {
            ++choice;
            if (choice == 1 ||
                std::uniform_int_distribution<>(1, choice)(*rng) == 1) {
              result.back() = {i, j};
            }
          }
        });
  }
  return result;
}

}  // namespace labmaze
}  // namespace deepmind
