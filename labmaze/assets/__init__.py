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

"""LabMaze texture assets."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import collections
import os
import sys

ROOT_DIR = sys.modules[__name__].__path__[0]

SKY_STYLES = ('sky_01', 'sky_02', 'sky_03')

SkyBox = collections.namedtuple(
    'SkyBox', ('left', 'right', 'up', 'down', 'front', 'back'))


def get_sky_texture_paths(style):
  if style not in SKY_STYLES:
    raise ValueError('`style` should be one of {}: got {!r}'.format(
        SKY_STYLES, style))
  return SkyBox(left=os.path.join(ROOT_DIR, style, 'lf.png'),
                right=os.path.join(ROOT_DIR, style, 'rt.png'),
                up=os.path.join(ROOT_DIR, style, 'up.png'),
                down=os.path.join(ROOT_DIR, style, 'dn.png'),
                front=os.path.join(ROOT_DIR, style, 'ft.png'),
                back=os.path.join(ROOT_DIR, style, 'bk.png'))


MAZE_STYLES = ('style_01', 'style_02', 'style_03', 'style_04', 'style_05')

WALL_TEXTURES = {
    'style_01': ('blue', 'cerise', 'green_bright', 'green', 'purple',
                 'red_bright', 'red', 'yellow'),
    'style_02': ('blue_bright', 'blue', 'dblue', 'lgreen', 'purple',
                 'yellow_bright', 'yellow'),
    'style_03': ('blue', 'cyan', 'gray_bright', 'gray',
                 'orange_bright', 'orange', 'purple'),
    'style_04': ('cerise', 'green_bright', 'green', 'purple',
                 'red_bright', 'red'),
    'style_05': ('red_bright', 'red', 'yellow_bright', 'yellow')
}


def get_wall_texture_paths(style):
  if style not in MAZE_STYLES:
    raise ValueError('`style` should be one of {}: got {!r}'.format(
        MAZE_STYLES, style))
  return {name: os.path.join(ROOT_DIR, style, 'wall_{:s}_d.png'.format(name))
          for name in WALL_TEXTURES[style]}


FLOOR_TEXTURES = {
    'style_01': ('blue_bright', 'blue', 'blue_team', 'orange_bright', 'orange',
                 'red_team'),
    'style_02': ('blue_bright', 'blue', 'green_bright', 'green', 'orange'),
    'style_03': ('blue_bright', 'blue', 'green_bright', 'green', 'orange',
                 'purple', 'red'),
    'style_04': ('blue_bright', 'blue', 'cyan', 'dorange', 'orange_bright',
                 'orange'),
    'style_05': ('blue_bright', 'blue', 'orange_bright', 'orange')
}


def get_floor_texture_paths(style):
  if style not in MAZE_STYLES:
    raise ValueError('`style` should be one of {}: got {!r}'.format(
        MAZE_STYLES, style))
  return {name: os.path.join(ROOT_DIR, style, 'floor_{:s}_d.png'.format(name))
          for name in FLOOR_TEXTURES[style]}
