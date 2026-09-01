# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.20)
project(FORDLINK VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# LINK owns the shared diagnostic engines. Keep its tests out of this small
# product skeleton; FORDLINK's own smoke test remains enabled below.
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(src/link EXCLUDE_FROM_ALL)
set(BUILD_TESTING ON CACHE BOOL "" FORCE)
enable_testing()

add_library(fordlink-core STATIC src/fordlink.c)
add_library(FORDLINK::Core ALIAS fordlink-core)
target_include_directories(fordlink-core PUBLIC include)
target_link_libraries(fordlink-core PUBLIC LINK::Core)

add_executable(fordlink-smoke tests/test_smoke.c)
target_link_libraries(fordlink-smoke PRIVATE FORDLINK::Core)
add_test(NAME fordlink-smoke COMMAND fordlink-smoke)
