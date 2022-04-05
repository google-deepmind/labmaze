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

workspace(name = "labmaze")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "bazel_skylib",
    sha256 = "e8cedbf5f7572bdb01f7359baa4b874128c1f4089b42d9735aa4dbc59df73846",
    strip_prefix = "bazel-skylib-1.2.1",
    url = "https://github.com/bazelbuild/bazel-skylib/archive/1.2.1.zip",
)

http_archive(
    name = "com_google_absl",
    sha256 = "a4567ff02faca671b95e31d315bab18b42b6c6f1a60e91c6ea84e5a2142112c2",
    strip_prefix = "abseil-cpp-20211102.0",
    url = "https://github.com/abseil/abseil-cpp/archive/20211102.0.zip",
)

http_archive(
    name = "com_google_googletest",
    sha256 = "353571c2440176ded91c2de6d6cd88ddd41401d14692ec1f99e35d013feda55a",
    strip_prefix = "googletest-release-1.11.0",
    url = "https://github.com/google/googletest/archive/release-1.11.0.zip",
)

http_archive(
    name = "pybind11",
    build_file = "@//bazel:pybind11.BUILD",
    sha256 = "d1646e6f70d8a3acb2ddd85ce1ed543b5dd579c68b8fb8e9638282af20edead8",
    strip_prefix = "pybind11-2.9.2",
    url = "https://github.com/pybind/pybind11/archive/v2.9.2.zip",
)

new_local_repository(
    name = "python_headers",
    path = "/usr/include/python3.7",  # May be overwritten by setup.py.
    build_file = "@//bazel:python_headers.BUILD"
)

# This is a transitive external dependency of @com_google_absl//absl/strings.
# Bazel doesn't pull in transitive dependencies from external WORKSPACE files.
# (https://docs.bazel.build/versions/master/external.html#transitive-dependencies)
http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-main",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/main.zip"],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")
bazel_skylib_workspace()
