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

"""Tests for labmaze.assets."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from absl.testing import absltest
from labmaze import assets

# DO NOT REMOVE THIS LINE -- COPYBARA: from google_internal import resources


def get_file_contents(path):
  with open(path, 'rb') as f:
    return f.read()


class AssetsTest(absltest.TestCase):

  def testHasSkyTextures(self):
    for style in assets.SKY_STYLES:
      texture_paths = assets.get_sky_texture_paths(style)
      self.assertTrue(get_file_contents(texture_paths.left))
      self.assertTrue(get_file_contents(texture_paths.right))
      self.assertTrue(get_file_contents(texture_paths.up))
      self.assertTrue(get_file_contents(texture_paths.down))
      self.assertTrue(get_file_contents(texture_paths.front))
      self.assertTrue(get_file_contents(texture_paths.back))

  def testHasWallTextures(self):
    for style in assets.MAZE_STYLES:
      texture_paths = assets.get_wall_texture_paths(style)
      for texture_name in assets.WALL_TEXTURES[style]:
        self.assertTrue(get_file_contents(texture_paths[texture_name]))

  def testHasFloorTextures(self):
    for style in assets.MAZE_STYLES:
      texture_paths = assets.get_floor_texture_paths(style)
      for texture_name in assets.FLOOR_TEXTURES[style]:
        self.assertTrue(get_file_contents(texture_paths[texture_name]))


if __name__ == '__main__':
  absltest.main()
