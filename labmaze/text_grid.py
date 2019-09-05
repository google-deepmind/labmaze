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

"""An array-like object that represents a 2D text grid."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import sys

import numpy as np


class TextGrid(np.ndarray):
  """An array-like object that represents a 2D text grid.

  This object is constructed from a newline-delimited string (with a trailing
  newline character) and exposes an 2D array-like interface for character-wise
  access. The original string can be retrieved by performing a string-conversion
  on this object.
  """

  def __new__(cls, newline_delimited_string):
    raw = newline_delimited_string
    split = raw[:-1].split('\n')
    height = len(split)
    width = len(split[0]) if split else 0

    dtype = 'U1' if sys.version_info[0] >= 3 else 'S1'
    obj = super(TextGrid, cls).__new__(cls, shape=(height, width), dtype=dtype)
    obj[:, :] = tuple(tuple(line) for line in split)
    return obj

  def __str__(self):
    lines = [''.join(row) for row in self]
    lines.append('')
    return '\n'.join(lines)
