# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/dangelc/quevotan/algoritmoC++/algoritmoc++/_deps/json-src"
  "/home/dangelc/quevotan/algoritmoC++/algoritmoc++/_deps/json-build"
  "/home/dangelc/quevotan/algoritmoC++/algoritmoc++/_deps/json-subbuild/json-populate-prefix"
  "/home/dangelc/quevotan/algoritmoC++/algoritmoc++/_deps/json-subbuild/json-populate-prefix/tmp"
  "/home/dangelc/quevotan/algoritmoC++/algoritmoc++/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
  "/home/dangelc/quevotan/algoritmoC++/algoritmoc++/_deps/json-subbuild/json-populate-prefix/src"
  "/home/dangelc/quevotan/algoritmoC++/algoritmoc++/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/dangelc/quevotan/algoritmoC++/algoritmoc++/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp/${subDir}")
endforeach()
