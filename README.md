# LabMaze

**LabMaze** is a standalone release of the same maze generator that is used by
[DeepMind Lab](https://github.com/deepmind/lab), which is a backtracking maze
generator that can be configured to simplify paths and create rooms.

The primary purpose of LabMaze is to provide Python bindings to DeepMind Lab's
maze generator, so that the generated maze layout can be used to create
navigation tasks through other libraries. This package also includes art assets
for texturing the resulting maze environment in the same style as in DeepMind
Lab.

The maze generation algorithm and code was originally designed and authored by
Charlie Beattie at DeepMind.

## Citation

If you use LabMaze in your research, we suggest that you cite the
[DeepMind Lab paper](https://arxiv.org/abs/1612.03801) along with a link to the
this GitHub repository.

## Installation from PyPI

LabMaze contains C++ code that first needs to be compiled into a Python
extension. For Python 2.7, 3.5, 3.6, and 3.7 on x64 Linux, macOS, and Windows,
we provide
[built distributions (bdist)](https://packaging.python.org/glossary/#term-built-distribution)
that can be straightforwardly installed via:

```sh
pip install labmaze
```

We also upload a
[source distribution (sdist)](https://packaging.python.org/glossary/#term-source-distribution-or-sdist)
to PyPI, which would allow LabMaze to be installed via `pip install labmaze` on
other platforms as well. However, you will first need to
[install Bazel](https://docs.bazel.build/versions/master/install.html), along
with the requisite platform-specific build toolchain as documented in the linked
page.

## Installation from GitHub repository

Alternatively, you can also use `pip` to install directly from our GitHub
repository. You will first need to
[install Bazel](https://docs.bazel.build/versions/master/install.html), along
with the requisite platform-specific build toolchain as documented in the linked
page, then install via

```sh
pip install git+git://github.com/deepmind/labmaze
```

## Example usage: random mazes

To generate a random maze:

```python
import labmaze
maze = labmaze.RandomMaze(height=11, width=13, random_seed=42)
print(maze.entity_layer)

*************
*   *   *****
*   *   *****
*           *
*** *** *** *
*           *
*   *****   *
*   *****   *
*   *****   *
*   *****   *
*************
```

By default, maze wall is represented by the `'*'` token. The `RandomMaze` object
can be re-randomized:

```python
maze.regenerate()
print(maze.entity_layer)

*************
*******     *
*******     *
*     *     *
* *** ***** *
*   *       *
*   *** *** *
*       *   *
*   *   *   *
*   *       *
*************
```

We can also generate spawn locations and goal object locations, e.g. for use in
a navigation task. These locations are also re-randomized along with the maze
layout itself when the `regenerate()` method is called.

```python
maze = labmaze.RandomMaze(height=11, width=13, random_seed=42,
                          spawns_per_room=1, objects_per_room=1)
print(maze.entity_layer)

*************
*P  *P  *****
*   * G *****
* G         *
*** *** *** *
* P      PG *
*   *****   *
*   *****   *
*   *****   *
*G  *****   *
*************
```

By default, player spawn positions are represented by the `'P'` token, and the
goal positions are represented by the `'G'` token. In the example above, one
player spawn position has been generated for each "room" in the maze. One way to
use the layout generated above be to pick one of the four spawn points at
random, but place an object at all four goal positions.

The `RandomMaze` object also contains "variation layers" that specifies how
different sections of the maze should be textured:

```python
.............
.BBB.DDD.....
.BBB.DDD.....
.BBB.DDD.....
.............
.CCC.....AAA.
.CCC.....AAA.
.CCC.....AAA.
.CCC.....AAA.
.CCC.....AAA.
.............
```

The `'.'` token represents the "default" texturing style, while each of the
alphabetical token represents distinct styles. It is up to the user to decide
how translate the suggested layout into different textures.

## Example usage: fixed-layout mazes

LabMaze also supports fixed-layout mazes that are specified via a string:

```python
MAZE_LAYOUT = """
*********
*********
*********
***   ***
***   ***
***   ***
*********
"""[1:]
maze = labmaze.FixedMazeWithRandomGoals(entity_layer=MAZE_LAYOUT)
print(maze.entity_layer)

*********
*********
*********
***   ***
***   ***
***   ***
*********
```

Spawn and goal positions can also be either specified or randomized into a
fixed-layout maze. For example, if one fixed spawn point and one randomized goal
is required:

```python
MAZE_LAYOUT = """
*********
***   ***
***   ***
*** P ***
***   ***
***   ***
*********
"""[1:]
maze = labmaze.FixedMazeWithRandomGoals(entity_layer=MAZE_LAYOUT,
                                        num_spawns=1, num_objects=1)
print(maze.entity_layer)

*********
***   ***
***   ***
*** P ***
***   ***
***G  ***
*********

maze.regenerate()
print(maze.entity_layer)

*********
***   ***
***   ***
*** PG***
***   ***
***   ***
*********
```

More generally, any spawn or goal tokens present in the fixed-layout string are
left unmodified by the maze generator. If `num_objects` is greater than the
number of goal tokens already present in the string then additional ones are
randomly generated. The same goes for `num_spawns`.

## Disclaimer

This is not an official Google product.
