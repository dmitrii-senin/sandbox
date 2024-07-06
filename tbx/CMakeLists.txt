cmake_minimum_required(VERSION 3.21)

project(
  Tbx
  VERSION 0.0.1
  DESCRIPTION "All useful data structures and algorithms in one place."
  HOMEPAGE_URL "https://github.com/dmitrii-senin/sandbox/tree/main/tbx"
  LANGUAGES CXX
)

include(cmake/prevent_in_source_builds.cmake)
include(cmake/options.cmake)

set(TBX_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${TBX_DIR}/src/tbx)

add_subdirectory(src)
