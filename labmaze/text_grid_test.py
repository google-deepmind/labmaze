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

"""Tests for labmaze.TextGrid."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import copy

from absl.testing import absltest
import labmaze
import numpy as np
from six.moves import range
from six.moves import zip


class TextGridTest(absltest.TestCase):

  def testConstruction(self):
    original_string = 'abcd\nefgh\nijkl\n'
    maze = labmaze.TextGrid(original_string)
    self.assertEqual(str(maze), original_string)
    self.assertEqual(maze.shape, (3, 4))

    expected_array = [['a', 'b', 'c', 'd'],
                      ['e', 'f', 'g', 'h'],
                      ['i', 'j', 'k', 'l']]
    np.testing.assert_array_equal(maze, expected_array)

    # Test that iteration works.
    for actual_row, expected_row in zip(maze, expected_array):
      for actual_col, expected_col in zip(actual_row, expected_row):
        self.assertEqual(actual_col.strip(), expected_col.strip())

    # Test that indexed access works.
    height = len(expected_array)
    width = len(expected_array[0])
    for y in range(height):
      for x in range(width):
        self.assertEqual(maze[y, x].strip(), expected_array[y][x].strip())

  def testAssignment(self):
    original_string = 'abcd\nefgh\nijkl\n'
    maze = labmaze.TextGrid(original_string)
    maze[:2, :2] = '#'

    expected_string = '##cd\n##gh\nijkl\n'
    expected_array = [['#', '#', 'c', 'd'],
                      ['#', '#', 'g', 'h'],
                      ['i', 'j', 'k', 'l']]
    self.assertEqual(str(maze), expected_string)
    np.testing.assert_array_equal(maze, expected_array)

  def testCopying(self):
    original_string = '1234\n5678\n'
    maze = labmaze.TextGrid(original_string)
    copied = copy.copy(maze)
    self.assertEqual(type(maze), type(copied))
    self.assertEqual(str(maze), str(copied))
    np.testing.assert_array_equal(maze, copied)
    deepcopied = copy.deepcopy(maze)
    self.assertEqual(type(maze), type(deepcopied))
    self.assertEqual(str(maze), str(deepcopied))
    np.testing.assert_array_equal(maze, deepcopied)


if __name__ == '__main__':
  absltest.main()
