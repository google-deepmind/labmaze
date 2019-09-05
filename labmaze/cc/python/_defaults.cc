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

#include "labmaze/cc/defaults.h"
#include "pybind11/pybind11.h"
#include "pybind11/pytypes.h"

namespace deepmind {
namespace labmaze {

namespace py = pybind11;

PYBIND11_MODULE(_defaults, m) {
  m.add_object("MAX_ROOMS", py::int_(defaults::kMaxRooms));
  m.add_object("ROOM_MIN_SIZE", py::int_(defaults::kRoomMinSize));
  m.add_object("ROOM_MAX_SIZE", py::int_(defaults::kRoomMaxSize));
  m.add_object("RETRY_COUNT", py::int_(defaults::kRetryCount));
  m.add_object("EXTRA_CONNECTION_PROBABILITY",
               py::float_(defaults::kExtraConnectionProbability));
  m.add_object("MAX_VARIATIONS", py::int_(defaults::kMaxVariations));
  m.add_object("HAS_DOORS", py::bool_(defaults::kHasDoors));
  m.add_object("SIMPLIFY", py::bool_(defaults::kSimplify));
  m.add_object("SPAWN_COUNT", py::int_(defaults::kSpawnCount));
  m.add_object("SPAWN_TOKEN", py::str(defaults::kSpawnToken));
  m.add_object("OBJECT_COUNT", py::int_(defaults::kObjectCount));
  m.add_object("OBJECT_TOKEN", py::str(defaults::kObjectToken));
}

}  // namespace labmaze
}  // namespace deepmind
