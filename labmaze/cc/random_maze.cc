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

#include "labmaze/cc/random_maze.h"

#include <random>
#include <string>

namespace deepmind {
namespace labmaze {

RandomMaze::RandomMaze(int height, int width,
                       int max_rooms, int room_min_size, int room_max_size,
                       int retry_count, double extra_connection_probability,
                       int max_variations, bool has_doors, bool simplify,
                       int spawns_per_room, absl::string_view spawn_token,
                       int objects_per_room, absl::string_view object_token,
                       std::mt19937_64::result_type random_seed)
    : maze_size_{height, width},
      maze_params_{},
      extra_connection_probability_{extra_connection_probability},
      max_variations_{max_variations},
      has_doors_{has_doors},
      simplify_{simplify},
      spawns_per_room_{spawns_per_room},
      spawn_token_{spawn_token},
      objects_per_room_{objects_per_room},
      object_token_{object_token},
      prng_{random_seed},
      maze_{maze_size_} {
  maze_params_.min_size = Size{room_min_size, room_min_size};
  maze_params_.max_size = Size{room_max_size, room_max_size};
  maze_params_.retry_count = retry_count;
  maze_params_.max_rects = max_rooms;
  maze_params_.density = 1.0;
  Regenerate();
}

void RandomMaze::Regenerate() {
  maze_ = TextMaze(maze_size_);
  // Create random rooms.
  const auto rects = MakeSeparateRectangles(maze_.Area(), maze_params_, &prng_);
  const auto num_rooms = rects.size();
  for (unsigned int r = 0; r < num_rooms; ++r) {
    maze_.VisitMutableIntersection(TextMaze::kEntityLayer, rects[r],
                                   [this, r](int i, int j, char* cell) {
                                     *cell = ' ';
                                     maze_.SetCellId({i, j}, r + 1);
                                   });
  }

  // Fill the vacant space with corridors.
  FillSpaceWithMaze(num_rooms + 1, 0, &maze_, &prng_);

  // Connect adjacent regions at least once.
  auto conns =
      RandomConnectRegions(-1, extra_connection_probability_, &maze_, &prng_);

  // Simplify the maze_ if requested.
  if (simplify_) {
    RemoveDeadEnds(' ', '*', {}, &maze_);
    RemoveAllHorseshoeBends('*', {}, &maze_);
  }

  // Add variations.
  maze_.VisitMutable(
      TextMaze::kVariationsLayer,
      [this, num_rooms](int i, int j, char* cell) {
        auto id = maze_.GetCellId({i, j});
        if (id > 0 && id <= num_rooms) {
          *cell = 'A' + (id - 1) % max_variations_;
        }
      });

  // Add entities and spawn points.
  AddNEntitiesToEachRoom(rects, spawns_per_room_, spawn_token_[0], ' ', &maze_,
                         &prng_);
  AddNEntitiesToEachRoom(rects, objects_per_room_, object_token_[0], ' ',
                         &maze_, &prng_);

  // Set each connection cell connection type.
  for (const auto& conn : conns) {
    char connection_type;
    // Set to wall if connected to nowhere.
    if (maze_.GetCell(TextMaze::kEntityLayer,
                      conn.first + conn.second) == '*') {
      connection_type = '*';
    } else if (has_doors_) {
      connection_type = (conn.second.d_col == 0) ? 'H' : 'I';
    } else {
      connection_type = ' ';
    }
    maze_.SetCell(TextMaze::kEntityLayer, conn.first, connection_type);
  }
}

std::string RandomMaze::EntityLayer() const {
  return std::string(maze_.Text(TextMaze::kEntityLayer));
}

std::string RandomMaze::VariationsLayer() const {
  return std::string(maze_.Text(TextMaze::kVariationsLayer));
}

}  // namespace labmaze
}  // namespace deepmind
