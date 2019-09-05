# Copyright 2019 DeepMind Technologies Limited.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================
#
# Description:
#   C++ components for LabMaze. Intended for internal usage only.
#   Please use the Python API provided directly under the LabMaze package.

# DO NOT REMOVE THIS LINE -- COPYBARA: load("@google_internal", "cc_hermetic_test")

licenses(["notice"])  # Apache 2.0 License

cc_library(
    name = "defaults",
    hdrs = ["defaults.h"],
    visibility = ["@//labmaze/cc/python:__pkg__"],
)

cc_library(
    name = "algorithm",
    srcs = ["algorithm.cc"],
    hdrs = ["algorithm.h"],
    deps = [
        ":char_grid",
        ":flood_fill",
        ":text_maze",
    ],
)

cc_test(
    name = "algorithm_test",
    size = "small",
    srcs = ["algorithm_test.cc"],
    tags = ["manual"],  # Different C++ library implementations may generate different results.
    deps = [
        ":algorithm",
        ":text_maze",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "char_grid",
    srcs = ["char_grid.cc"],
    hdrs = ["char_grid.h"],
    deps = [
        ":logging",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "char_grid_test",
    size = "small",
    srcs = ["char_grid_test.cc"],
    deps = [
        ":char_grid",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "flood_fill",
    srcs = ["flood_fill.cc"],
    hdrs = ["flood_fill.h"],
    deps = [":text_maze"],
)

cc_test(
    name = "flood_fill_test",
    size = "small",
    srcs = ["flood_fill_test.cc"],
    deps = [
        ":algorithm",
        ":flood_fill",
        ":text_maze",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "random_maze",
    srcs = ["random_maze.cc"],
    hdrs = ["random_maze.h"],
    visibility = ["@//labmaze/cc/python:__pkg__"],
    deps = [
        ":algorithm",
        ":text_maze",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "random_maze_test",
    srcs = ["random_maze_test.cc"],
    tags = ["manual"],  # Different C++ library implementations may generate different results.
    deps = [
        ":defaults",
        ":random_maze",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "text_maze",
    srcs = ["text_maze.cc"],
    hdrs = ["text_maze.h"],
)

cc_library(
    name = "logging",
    hdrs = ["logging.h"],
)

cc_test(
    name = "text_maze_test",
    size = "small",
    srcs = ["text_maze_test.cc"],
    deps = [
        ":text_maze",
        "@com_google_googletest//:gtest_main",
    ],
)
