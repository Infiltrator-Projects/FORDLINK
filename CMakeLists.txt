# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.20)
project(FORDLINK VERSION 0.1.1 LANGUAGES C)

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

add_executable(fordlink-obd2 tests/test_obd2.c)
target_link_libraries(fordlink-obd2 PRIVATE FORDLINK::Core)
add_test(NAME fordlink-obd2 COMMAND fordlink-obd2)

add_executable(fordlink-generic-diagnostics tests/test_generic_diagnostics.c)
target_link_libraries(fordlink-generic-diagnostics PRIVATE FORDLINK::Core)
add_test(NAME fordlink-generic-diagnostics COMMAND fordlink-generic-diagnostics)

include(GNUInstallDirs)

add_executable(fordlink app/linux/main.c)
target_link_libraries(fordlink PRIVATE FORDLINK::Core)

install(TARGETS fordlink RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES README.md docs/OBD2.md docs/GENERIC_BASELINE.md
        DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/fordlink")

set(CPACK_PACKAGE_NAME "fordlink")
set(CPACK_PACKAGE_VENDOR "FORDLINK Project")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ford diagnostics with a generic LINK standards baseline")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "Infiltrator Projects")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Infiltrator Projects")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include(CPack)
