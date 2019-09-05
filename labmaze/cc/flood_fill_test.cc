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

#include <tuple>

#include "gtest/gtest.h"
#include "labmaze/cc/algorithm.h"
#include "labmaze/cc/text_maze.h"

namespace deepmind {
namespace labmaze {
namespace {

TEST(FloodFillTest, DistanceLinear) {
  TextMaze maze = FromCharGrid(CharGrid("    "));
  FloodFill fill_info(maze, TextMaze::kEntityLayer, {0, 0}, {'*'});
  maze.Area().Visit([&fill_info](int i, int j) {
    const int distances[1][4] = {{0, 1, 2, 3}};
    EXPECT_EQ(distances[i][j], fill_info.DistanceFrom({i, j}));
  });

  fill_info = FloodFill(maze, TextMaze::kEntityLayer, {0, 1}, {'*'});
  maze.Area().Visit([&fill_info](int i, int j) {
    const int distances[1][4] = {{1, 0, 1, 2}};
    EXPECT_EQ(distances[i][j], fill_info.DistanceFrom({i, j}));
  });

  fill_info = FloodFill(maze, TextMaze::kEntityLayer, {0, 3}, {'*'});
  maze.Area().Visit([&fill_info](int i, int j) {
    const int distances[1][4] = {{3, 2, 1, 0}};
    EXPECT_EQ(distances[i][j], fill_info.DistanceFrom({i, j}));
  });

  fill_info = FloodFill(maze, TextMaze::kEntityLayer, {-1, 0}, {'*'});
  maze.Area().Visit([&fill_info](int i, int j) {
    EXPECT_EQ(-1, fill_info.DistanceFrom({i, j}));
  });
}

TEST(FloodFillTest, DistanceManhatten) {
  TextMaze maze =
      FromCharGrid(CharGrid("   \n"
                            "   \n"
                            "   \n"));

  FloodFill fill_info(maze, TextMaze::kEntityLayer, {0, 0}, {'*'});

  maze.Area().Visit([&fill_info](int i, int j) {
    const int distances[3][3] = {
        {0, 1, 2},  //
        {1, 2, 3},  //
        {2, 3, 4},  //
    };
    EXPECT_EQ(distances[i][j], fill_info.DistanceFrom({i, j}));
  });

  fill_info = FloodFill(maze, TextMaze::kEntityLayer, {1, 1}, {'*'});
  maze.Area().Visit([&fill_info](int i, int j) {
    const int distances[3][3] = {
        {2, 1, 2},  //
        {1, 0, 1},  //
        {2, 1, 2},
    };
    EXPECT_EQ(distances[i][j], fill_info.DistanceFrom({i, j}));
  });
}

TEST(FloodFillTest, DistanceWallsConnected) {
  TextMaze maze =
      FromCharGrid(CharGrid(" * \n"
                            "   \n"
                            "   \n"));

  FloodFill fill_info(maze, TextMaze::kEntityLayer, {0, 0}, {'*'});
  maze.Area().Visit([&fill_info](int i, int j) {
    const int distances[3][3] = {
        {0, -1, 4},  //
        {1, 2, 3},   //
        {2, 3, 4},
    };
    EXPECT_EQ(distances[i][j], fill_info.DistanceFrom({i, j}));
  });
}

TEST(FloodFillTest, DistanceWallsSeparated) {
  TextMaze maze =
      FromCharGrid(CharGrid(" * \n"
                            " * \n"
                            " * \n"));

  FloodFill fill_info(maze, TextMaze::kEntityLayer, {0, 0}, {'*'});
  maze.Area().Visit([&fill_info](int i, int j) {
    const int distances[3][3] = {
        {0, -1, -1},  //
        {1, -1, -1},  //
        {2, -1, -1},
    };
    EXPECT_EQ(distances[i][j], fill_info.DistanceFrom({i, j}));
  });
}

TEST(FloodFillTest, ShortestPathFrom) {
  TextMaze maze =
      FromCharGrid(CharGrid(" * \n"
                            " * \n"
                            "   \n"));
  FloodFill fill_info(maze, TextMaze::kEntityLayer, {0, 0}, {'*'});
  maze.Area().Visit([&fill_info](int i, int j) {
    const int distances[3][3] = {
        {0, -1, 6},  //
        {1, -1, 5},  //
        {2, 3, 4},
    };
    EXPECT_EQ(distances[i][j], fill_info.DistanceFrom({i, j}));
  });
  std::mt19937_64 gen(10);
  auto path = fill_info.ShortestPathFrom({1, 2}, &gen);
  Pos pos[] = {{1, 2}, {2, 2}, {2, 1}, {2, 0}, {1, 0}, {0, 0}};
  ASSERT_EQ(std::distance(std::begin(pos), std::end(pos)), path.size());
  EXPECT_TRUE(std::equal(
      path.begin(), path.end(), std::begin(pos), [](Pos lhs, Pos rhs) {
        return std::tie(lhs.row, lhs.col) == std::tie(rhs.row, rhs.col);
      }));

  Pos fill[] = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, {1, 2}, {0, 2}};
  fill_info.Visit([&fill](int i, int j, int distance) {
    EXPECT_EQ(i, fill[distance].row);
    EXPECT_EQ(j, fill[distance].col);
  });
}

TEST(FloodFillTest, ShortestPathFromRandom) {
  TextMaze maze({20, 20});
  FloodFill fill_info(maze, TextMaze::kEntityLayer, {19, 19}, {});
  std::mt19937_64 gen(10);
  auto path = fill_info.ShortestPathFrom({0, 0}, &gen);
  Pos prev = {-1, 0};
  for (auto p : path) {
    EXPECT_TRUE((p.row == prev.row + 1 && p.col == prev.col) ||
                (p.row == prev.row && p.col == prev.col + 1));
    prev = p;
  }
}

}  // namespace
}  // namespace labmaze
}  // namespace deepmind
