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

#include "labmaze/cc/text_maze.h"

namespace deepmind {
namespace labmaze {

TextMaze::TextMaze(Size extents) : area_{{0, 0}, extents} {
  std::string level_layer(area_.size.height * (area_.size.width + 1), '*');
  std::string variations_layer(area_.size.height * (area_.size.width + 1), '.');
  for (int i = 0; i < area_.size.height; ++i) {
    int text_idx = ToTextIdx(i, area_.size.width);
    level_layer[text_idx] = '\n';
    variations_layer[text_idx] = '\n';
  }
  text_[kEntityLayer] = std::move(level_layer);
  text_[kVariationsLayer] = std::move(variations_layer);
  ids_.assign(area_.size.height * area_.size.width, 0);
}

enum OrthoRotation {
  kDontRotate,
  kRotateClockwise,
  kRotate180,
  kRotateCounterclockwise,
  kNumOrthoRotations,
};

// Rotation + translation-as-a-percent-of-height-and-width.
// For a RotationMatrix2D matrix `r`,
//   (rotated_i, rotated_j) =
//      (r[0][0] * i + r[1][0] * j + r[2][0] * (height - 1),
//       r[0][1] * i + r[1][1] * j + r[2][1] * (width - 1))
// The translation is used to keep the top-left corner at (0,0).
typedef int RotationMatrix2D[3][2];

// Rotation + translation matrices for rotations that are a multiple of 90
// degrees.
static const RotationMatrix2D ROTATION_MATRICES_2D[kNumOrthoRotations] = {
  // kDontRotate (identity)
  { {  1,  0 }, {  0,  1 }, { 0, 0 } },

  // kRotateClockwise
  //     old      new
  //     j->
  //     012      0123
  // i 0 ABC    0 JGDA
  // | 1 DEF    1 KHEB
  // | 2 GHI    2 LIFC
  // v 3 JKL
  //
  // A 0,0 ==> 0,0  + 0,3  add on height-1
  // B 0,1 ==> 1,0  + 0,3  j --> i
  // D 1,0 ==> 0,-1 + 0,3  i --> -j
  { {  0, -1 }, {  1,  0 }, { 0, 1 } },

  // kRotate180
  //     old      new
  //     j->
  //     012      012
  // i 0 ABC    0 LKJ
  // | 1 DEF    1 IHG
  // | 2 GHI    2 FED
  // v 3 JKL    3 CBA
  //
  // A 0,0 ==>  0,0  + 3,2  add on (width-1, height-1)
  // B 0,1 ==>  0,-1 + 3,2  j --> -i
  // D 1,0 ==> -1,0  + 3,2  i --> j
  { { -1,  0 }, {  0, -1 }, { 1, 1 } },

  // kRotateCounterclockwise
  //     old      new
  //     j->
  //     012      0123
  // i 0 ABC    0 CFIL
  // | 1 DEF    1 BEHK
  // | 2 GHI    2 ADGJ
  // v 3 JKL
  //
  // A 0,0 ==>  0,0  + 2,0  add on width-1
  // B 0,1 ==> -1,0  + 2,0  j --> -i
  // D 1,0 ==>  0,1  + 2,0  i --> j
  { {  0,  1 }, { -1,  0 }, { 1, 0 } },
};

static OrthoRotation CalcOrthoRotation(int rotation) {
  const int modRotation = rotation % kNumOrthoRotations;
  return static_cast<OrthoRotation>(
      modRotation < 0 ? modRotation + kNumOrthoRotations : modRotation);
}

TextMaze TextMaze::Rotate(int rotation) const {
  // There are four possible rotations. Ignore the one that's identity.
  const OrthoRotation ortho = CalcOrthoRotation(rotation);
  const RotationMatrix2D& r = ROTATION_MATRICES_2D[ortho];

  const Size extents{
      rotation % 2 == 0 ? area_.size.height : area_.size.width,
      rotation % 2 == 0 ? area_.size.width : area_.size.height};
  TextMaze m(extents);

  // Fill in the new maze with the rotated values.
  Visit(kEntityLayer, [&](int i, int j, char entityValue) {
    const Pos oldPos{i, j};
    const Pos newPos{
      r[0][0] * i + r[1][0] * j + r[2][0] * (extents.height - 1),
      r[0][1] * i + r[1][1] * j + r[2][1] * (extents.width - 1)
    };
    m.SetCell(kEntityLayer, newPos, entityValue);
    m.SetCell(kVariationsLayer, newPos, GetCell(kVariationsLayer, oldPos));
    m.SetCellId(newPos, GetCellId(oldPos));
  });
  return m;
}

}  // namespace labmaze
}  // namespace deepmind
