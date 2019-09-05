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

"""Tests for labmaze.fixed_maze."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from absl.testing import absltest
from absl.testing import parameterized
from labmaze import fixed_maze
import numpy as np
from six.moves import range

_WIDTH = 9
_HEIGHT = 7
_EMPTY_CELL = ' '
_WALL = '*'
_SPAWN_TOKEN = '%'
_OBJECT_TOKEN = '$'

_MAZE_1 = ('* * * * *\n'
           ' * * * * \n'
           '* * * * *\n'
           ' * * * * \n'
           '* * * * *\n'
           ' * * * * \n'
           '* * * * *\n')

_MAZE_2 = ('* * * * *\n'
           ' *%* *$* \n'
           '* *$* * *\n'
           ' *%* *%* \n'
           '* * *$* *\n'
           ' *$* * * \n'
           '* * *$* *\n')
_MAZE_2_SPAWNS = ((1, 2), (3, 2), (3, 6))
_MAZE_2_OBJECTS = ((1, 6), (2, 3), (4, 5), (5, 2), (6, 5))


class FixedMazeWithRandomGoalsTest(parameterized.TestCase):

  def testCorrectSize(self):
    maze = fixed_maze.FixedMazeWithRandomGoals(entity_layer=_MAZE_1)
    self.assertEqual(maze.height, _HEIGHT)
    self.assertEqual(maze.width, _WIDTH)

  def testCorrectEntityLayer(self):
    maze = fixed_maze.FixedMazeWithRandomGoals(
        entity_layer=_MAZE_2,
        num_spawns=0, spawn_token=_SPAWN_TOKEN,
        num_objects=0, object_token=_OBJECT_TOKEN)
    maze_chars = _MAZE_1.split('\n')
    for y in range(maze.height):
      for x in range(maze.width):
        self.assertEqual(maze.entity_layer[y, x], maze_chars[y][x])

  def assert_consistent_maze(self, maze, num_spawns, num_objects,
                             required_spawns=(), required_objects=()):
    spawns_found = 0
    objects_found = 0
    for y in range(maze.height):
      for x in range(maze.width):
        if y % 2 == x % 2:
          self.assertEqual(maze.entity_layer[y, x], _WALL)
        else:
          self.assertIn(maze.entity_layer[y, x],
                        (_EMPTY_CELL, _SPAWN_TOKEN, _OBJECT_TOKEN))
          if maze.entity_layer[y, x] == _SPAWN_TOKEN:
            spawns_found += 1
            if num_spawns < len(required_spawns):
              self.assertIn((y, x), required_spawns)
          elif maze.entity_layer[y, x] == _OBJECT_TOKEN:
            objects_found += 1
            if num_objects < len(required_objects):
              self.assertIn((y, x), required_objects)

    if num_spawns >= len(required_spawns):
      for required_spawn in required_spawns:
        self.assertEqual(maze.entity_layer[required_spawn], _SPAWN_TOKEN)
    if num_objects >= len(required_objects):
      for required_object in required_objects:
        self.assertEqual(maze.entity_layer[required_object], _OBJECT_TOKEN)

    self.assertEqual(spawns_found, num_spawns)
    self.assertEqual(objects_found, num_objects)

  def testConsistentMazeUnconstrained(self):
    num_spawns = 2
    num_objects = 3
    maze = fixed_maze.FixedMazeWithRandomGoals(
        entity_layer=_MAZE_1,
        num_spawns=num_spawns, spawn_token=_SPAWN_TOKEN,
        num_objects=num_objects, object_token=_OBJECT_TOKEN,
        random_state=np.random.RandomState(123))
    self.assert_consistent_maze(maze, num_spawns, num_objects)
    for _ in range(5):
      old_entity_layer = maze.entity_layer.copy()
      maze.regenerate()
      self.assertFalse((maze.entity_layer == old_entity_layer).all())
      self.assert_consistent_maze(maze, num_spawns, num_objects)

  @parameterized.named_parameters(
      ('underconstrained', 1, 2),
      ('overconstrained', 4, 7))
  def testConsistentMazeConstrained(self, num_spawns, num_objects):
    maze = fixed_maze.FixedMazeWithRandomGoals(
        entity_layer=_MAZE_2,
        num_spawns=num_spawns, spawn_token=_SPAWN_TOKEN,
        num_objects=num_objects, object_token=_OBJECT_TOKEN,
        random_state=np.random.RandomState(234))

    self.assert_consistent_maze(maze, num_spawns, num_objects,
                                required_spawns=_MAZE_2_SPAWNS,
                                required_objects=_MAZE_2_OBJECTS)
    for _ in range(5):
      old_entity_layer = maze.entity_layer.copy()
      maze.regenerate()
      self.assertFalse((maze.entity_layer == old_entity_layer).all())
      self.assert_consistent_maze(maze, num_spawns, num_objects,
                                  required_spawns=_MAZE_2_SPAWNS,
                                  required_objects=_MAZE_2_OBJECTS)


if __name__ == '__main__':
  absltest.main()
