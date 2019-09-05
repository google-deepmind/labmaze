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

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "labmaze/cc/defaults.h"

// This file provides a quick sanity check for the C++ bindings layer.
// Most of the functionality tests are implemented against the Python API
// that is externally exposed.

namespace deepmind {
namespace labmaze {

// Golden tests are brittle since the output depends on the specific
// implementation of various random algorithms. When moving platforms, new
// golden output data will most likely need to be generated.

TEST(RandomMazeTest, TestGolden7x9Maze) {
  RandomMaze maze{
      7, 9,
      defaults::kMaxRooms, defaults::kRoomMinSize, defaults::kRoomMaxSize,
      defaults::kRetryCount, defaults::kExtraConnectionProbability,
      defaults::kMaxVariations, defaults::kHasDoors, defaults::kSimplify,
      defaults::kSpawnCount, defaults::kSpawnToken,
      defaults::kObjectCount, defaults::kObjectToken,
      12345  /* random seed */
  };

  std::string entity_layer = maze.EntityLayer();
  std::string variations_layer = maze.VariationsLayer();

  EXPECT_TRUE(entity_layer ==
                  "*********\n"
                  "*********\n"
                  "*********\n"
                  "***   ***\n"
                  "***   ***\n"
                  "***   ***\n"
                  "*********\n" ||
              entity_layer ==
                  "*********\n"
                  "*********\n"
                  "*********\n"
                  "*     ***\n"
                  "*     ***\n"
                  "*     ***\n"
                  "*********\n");

  EXPECT_TRUE(variations_layer ==
                  ".........\n"
                  ".........\n"
                  ".........\n"
                  "...AAA...\n"
                  "...AAA...\n"
                  "...AAA...\n"
                  ".........\n" ||

              variations_layer ==
                  ".........\n"
                  ".........\n"
                  ".........\n"
                  ".AAAAA...\n"
                  ".AAAAA...\n"
                  ".AAAAA...\n"
                  ".........\n");
}

}  // namespace labmaze
}  // namespace deepmind
