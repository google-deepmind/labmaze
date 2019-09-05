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

"""Text mazes with a fixed, prespecified layout."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import itertools
from labmaze import base
from labmaze import defaults
from labmaze import text_grid
import numpy as np
from six.moves import range

_EMPTY_CELL = ' '


class FixedMazeWithRandomGoals(base.BaseMaze):
  """A maze with fixed layout but randomized spawn points and object positions.

  The maze's layout is prespecified through a newline-terminated string. At
  each call to `regenerate`, the spawn points and object positions are randomly
  drawn from the empty cells within the specified maze layout. The number of
  spawn points and object positions generated is user-specifiable, but is fixed
  across calls to `regenerate`.

  Optionally, the user can also constrain the spawn points and object positions
  to be drawn from a predetermined set. This is done by including the spawn
  and/or object tokens directly into the maze layout string. If the requested
  number of spawns or objects is greater than the number of the respective
  tokens present in the layout string, then additional spawns/objects are
  randomly generated into the remaining empty cells in the layout string.
  """

  def __init__(self, entity_layer, variations_layer=None,
               num_spawns=None,
               spawn_token=defaults.SPAWN_TOKEN,
               num_objects=None,
               object_token=defaults.OBJECT_TOKEN,
               random_state=None):
    """Initializes this maze object.

    Args:
      entity_layer: A string specifying the layout of this maze. The lines in
        this string should be of equal length, and the string should terminate
        with a newline character. Maze walls are represented by the '*'
        character. Optionally, candidate positions for spawn points and object
        locations can be specified by adding the appropriate tokens to the
        desired locations in the string. See this class' docstring for details.
      variations_layer: A string specifying the variations layer for this maze.
        This is used to change the visual appearance across various parts of
        the maze. See the docstring for `variations_layer` property for details.
      num_spawns: The total number of spawn points to be generated within this
        maze. If `None`, this maze will use all spawn tokens present in the
        `entity_layer` string (i.e. no randomization).
      spawn_token: The character that is used to represent spawn points in this
        text maze.
      num_objects: The total number of object positions to be generated within
        this maze. If `None`, this maze will use all object tokens present in
        the `entity_layer` string (i.e. no randomization).
      object_token: The character that is used to represent object locations in
        this text maze.
      random_state: A `numpy.random.RandomState` object.

    Raises:
      ValueError: If `variations_layer` is not of the same shape as
        `entity_layer`, or if `spawn_token` or `object_token` is not exactly
        one character long.
    """
    self._entity_layer = text_grid.TextGrid(entity_layer)

    if variations_layer is not None:
      self._variations_layer = text_grid.TextGrid(variations_layer)
      if self._variations_layer.shape != self._entity_layer.shape:
        raise ValueError(
            '`variations_layer` is specified, but its shape is not the same as '
            '`entity_layer`: got {} vs. {}'
            .format(self._variations_layer.shape, self._entity_layer.shape))
    else:
      self._variations_layer = self._entity_layer.copy()
      self._variations_layer[:] = '.'

    self._max_variations = len(set(self._variations_layer.flatten())) - 1

    spawn_token = str(spawn_token)
    if len(spawn_token) != 1:
      raise ValueError('`spawn_token` should be a single character: '
                       'got {!r}'.format(spawn_token))
    else:
      self._spawn_token = spawn_token

    object_token = str(object_token)
    if len(object_token) != 1:
      raise ValueError('`object_token` should be a single character: '
                       'got {!r}'.format(object_token))
    else:
      self._object_token = object_token

    self._height, self._width = self._entity_layer.shape
    self._random_state = random_state or np.random

    self._empty_cells = []
    self._required_spawns = []
    self._required_objects = []

    for y in range(self._height):
      for x in range(self._width):
        if self._entity_layer[y, x] == _EMPTY_CELL:
          self._empty_cells.append((y, x))
        elif self._entity_layer[y, x] == self._spawn_token:
          self._required_spawns.append((y, x))
        elif self._entity_layer[y, x] == self._object_token:
          self._required_objects.append((y, x))

    if num_spawns is not None:
      self._num_spawns = num_spawns
    else:
      self._num_spawns = len(self._required_spawns)

    if num_objects is not None:
      self._num_objects = num_objects
    else:
      self._num_objects = len(self._required_objects)

    self.regenerate()

  def regenerate(self):
    for (y, x) in itertools.chain(
        self._empty_cells, self._required_spawns, self._required_objects):
      self._entity_layer[y, x] = _EMPTY_CELL

    if self._required_spawns:
      chosen_spawn_indices = self._random_state.choice(
          len(self._required_spawns),
          min(self._num_spawns, len(self._required_spawns)), replace=False)
      spawns = [self._required_spawns[i] for i in chosen_spawn_indices]
    else:
      spawns = []

    if self._required_objects:
      chosen_object_indices = self._random_state.choice(
          len(self._required_objects),
          min(self._num_objects, len(self._required_objects)), replace=False)
      objects = [self._required_objects[i] for i in chosen_object_indices]
    else:
      objects = []

    extra_spawns = max(0, self._num_spawns - len(self._required_spawns))
    extra_objects = max(0, self._num_objects - len(self._required_objects))
    if extra_spawns + extra_objects > 0:
      chosen_extra_indices = self._random_state.choice(
          len(self._empty_cells), extra_spawns + extra_objects, replace=False)
      extras = [self._empty_cells[i] for i in chosen_extra_indices]
      spawns += extras[:extra_spawns]
      objects += extras[extra_spawns:]

    for (y, x) in spawns:
      self._entity_layer[y, x] = self._spawn_token
    for (y, x) in objects:
      self._entity_layer[y, x] = self._object_token

  @property
  def entity_layer(self):
    return self._entity_layer

  @property
  def variations_layer(self):
    return self._variations_layer

  @property
  def height(self):
    return self._height

  @property
  def width(self):
    return self._width

  @property
  def max_variations(self):
    return self._max_variations

  @property
  def max_rooms(self):
    return 1

  @property
  def objects_per_room(self):
    return self._num_objects

  @property
  def spawn_token(self):
    return self._spawn_token

  @property
  def object_token(self):
    return self._object_token
