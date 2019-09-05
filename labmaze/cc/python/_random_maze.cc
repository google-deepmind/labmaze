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

#include <string>

#include "labmaze/cc/random_maze.h"
#include "pybind11/pybind11.h"
#include "pybind11/pytypes.h"

namespace deepmind {
namespace labmaze {

namespace py = pybind11;

PYBIND11_MODULE(_random_maze, m) {
  py::class_<RandomMaze> random_maze_class(m, "RandomMaze");
  random_maze_class
      .def(py::init<int, int, int, int, int, int, float, int, bool, bool, int,
                    std::string, int, std::string, int>(),
           py::arg("height"),
           py::arg("width"),
           py::arg("max_rooms"),
           py::arg("room_min_size"),
           py::arg("room_max_size"),
           py::arg("retry_count"),
           py::arg("extra_connection_probability"),
           py::arg("max_variations"),
           py::arg("has_doors"),
           py::arg("simplify"),
           py::arg("spawns_per_room"),
           py::arg("spawn_token"),
           py::arg("objects_per_room"),
           py::arg("object_token"),
           py::arg("random_seed"))
      .def("regenerate", &RandomMaze::Regenerate)
      .def_property_readonly("entity_layer", &RandomMaze::EntityLayer)
      .def_property_readonly("variations_layer", &RandomMaze::VariationsLayer);
}

}  // namespace labmaze
}  // namespace deepmind
