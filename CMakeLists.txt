# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.20)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" FORDLINK_PROJECT_VERSION)
string(STRIP "${FORDLINK_PROJECT_VERSION}" FORDLINK_PROJECT_VERSION)
if(NOT FORDLINK_PROJECT_VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+$")
  message(FATAL_ERROR "VERSION must contain a semantic major.minor.patch version")
endif()
project(FORDLINK VERSION "${FORDLINK_PROJECT_VERSION}" LANGUAGES C)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/fordlink/fordlink.h" FORDLINK_PUBLIC_VERSION_HEADER)
string(REGEX MATCH
  "#define[ \t]+FORDLINK_VERSION[ \t]+\"([0-9]+\\.[0-9]+\\.[0-9]+)\""
  FORDLINK_PUBLIC_VERSION_MATCH "${FORDLINK_PUBLIC_VERSION_HEADER}")
if(NOT FORDLINK_PUBLIC_VERSION_MATCH OR NOT CMAKE_MATCH_1 STREQUAL PROJECT_VERSION)
  message(FATAL_ERROR "VERSION and FORDLINK_VERSION must agree")
endif()
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(src/link EXCLUDE_FROM_ALL)
set(BUILD_TESTING ON CACHE BOOL "" FORCE)
enable_testing()

add_library(fordlink-core STATIC
  src/fordlink.c
  src/network.c
  src/module.c
  src/identity.c
  src/module_scan.c
  src/signal.c
  src/procedure.c)
add_library(FORDLINK::Core ALIAS fordlink-core)
target_include_directories(fordlink-core PUBLIC include)
target_compile_definitions(fordlink-core PRIVATE FORDLINK_VERSION="${PROJECT_VERSION}")
target_link_libraries(fordlink-core PUBLIC LINK::Core)

add_executable(fordlink-smoke tests/test_smoke.c)
target_compile_definitions(fordlink-smoke PRIVATE
  FORDLINK_EXPECTED_VERSION="${PROJECT_VERSION}"
  FORDLINK_EXPECTED_LINK_VERSION="0.14.94")
target_link_libraries(fordlink-smoke PRIVATE FORDLINK::Core)
add_test(NAME fordlink-smoke COMMAND fordlink-smoke)

add_executable(fordlink-network tests/test_network.c)
target_link_libraries(fordlink-network PRIVATE FORDLINK::Core)
add_test(NAME fordlink-network COMMAND fordlink-network)
add_executable(fordlink-modules tests/test_modules.c)
target_link_libraries(fordlink-modules PRIVATE FORDLINK::Core)
add_test(NAME fordlink-modules COMMAND fordlink-modules)
add_executable(fordlink-module-scan tests/test_module_scan.c)
target_link_libraries(fordlink-module-scan PRIVATE FORDLINK::Core)
add_test(NAME fordlink-module-scan COMMAND fordlink-module-scan)
add_executable(fordlink-signal tests/test_signal.c)
target_link_libraries(fordlink-signal PRIVATE FORDLINK::Core)
add_test(NAME fordlink-signal COMMAND fordlink-signal)
add_executable(fordlink-procedure tests/test_procedure.c)
target_link_libraries(fordlink-procedure PRIVATE FORDLINK::Core)
add_test(NAME fordlink-procedure COMMAND fordlink-procedure)

option(FORDLINK_BUILD_LINUX_GUI "Build native GTK4 GUI" OFF)
option(FORDLINK_BUILD_WINDOWS_DISCOVER "Build Win32 Discover GUI" ${WIN32})
include(GNUInstallDirs)

if(FORDLINK_BUILD_LINUX_GUI)
  if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "FORDLINK_BUILD_LINUX_GUI is Linux-only")
  endif()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4>=4.6)
  find_program(GLIB_COMPILE_RESOURCES_EXECUTABLE NAMES glib-compile-resources REQUIRED)
  set(FORDLINK_LINUX_RESOURCE_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/fordlink-linux-resources.c")
  add_custom_command(OUTPUT "${FORDLINK_LINUX_RESOURCE_SOURCE}"
    COMMAND "${GLIB_COMPILE_RESOURCES_EXECUTABLE}" --generate-source --c-name=fordlink_linux_resources
      "--target=${FORDLINK_LINUX_RESOURCE_SOURCE}" fordlink.gresource.xml
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/app/linux"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/app/linux/fordlink.gresource.xml"
      "${CMAKE_CURRENT_SOURCE_DIR}/app/linux/assets/fordlink-emblem.png"
    VERBATIM)
  add_executable(fordlink app/linux/main.c "${FORDLINK_LINUX_RESOURCE_SOURCE}")
  set_source_files_properties("${FORDLINK_LINUX_RESOURCE_SOURCE}" PROPERTIES COMPILE_OPTIONS "-w")
  target_link_libraries(fordlink PRIVATE FORDLINK::Core PkgConfig::GTK4)
  link_enable_linux_gtk_shell(fordlink UDEV_RULE_NAME "60-fordlink-openport2.rules")
  install(TARGETS fordlink RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
  install(FILES app/linux/assets/fordlink-emblem.png DESTINATION "${CMAKE_INSTALL_DATADIR}/pixmaps" RENAME fordlink.png)
  install(FILES app/linux/fordlink.desktop DESTINATION "${CMAKE_INSTALL_DATADIR}/applications")
  install(FILES README.md docs/FORD_NETWORKS.md docs/FORD_MODULES.md docs/FORD_DATA_MODEL.md
    DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/fordlink")
  set(CPACK_GENERATOR "DEB")
  set(CPACK_PACKAGE_NAME "fordlink")
  set(CPACK_PACKAGE_VENDOR "Infiltrator Projects")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ford diagnostics GUI using LINK")
  set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
  set(CPACK_PACKAGE_CONTACT "Infiltrator Projects")
  set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Infiltrator Projects")
  set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
  set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
  set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  include(CPack)
endif()

if(FORDLINK_BUILD_WINDOWS_DISCOVER)
  if(NOT WIN32)
    message(FATAL_ERROR "FORDLINK_BUILD_WINDOWS_DISCOVER is Windows-only")
  endif()
  link_add_windows_discover(fordlink-discover
    PRODUCT_NAME "FORDLINK" PRODUCT_SLUG "fordlink" WINDOW_CLASS "FORDLINKDiscoverWindow"
    PRODUCT_VERSION "${PROJECT_VERSION}" PRODUCT_COPYRIGHT "Copyright (C) 2026 Shannon Smith"
    PRODUCT_SUBTITLE "Ford Diagnostics"
    PRODUCT_DESCRIPTION "A C-first, open-source Ford vehicle diagnostics platform."
    PRODUCT_AUTHORS "Shannon Smith"
    PRODUCT_LICENSE_NAME "GPL-3.0-or-later"
    PRODUCT_WEBSITE "https://github.com/Infiltrator-Projects/FORDLINK"
    PRODUCT_ICON_PNG "${CMAKE_CURRENT_SOURCE_DIR}/app/ios/FORDLINK/Assets.xcassets/AppIcon.appiconset/AppIcon-60@3x.png")
  target_link_libraries(fordlink-discover PRIVATE FORDLINK::Core)
  if(MSVC)
    target_compile_options(fordlink-discover PRIVATE
      "/FI${CMAKE_CURRENT_SOURCE_DIR}/src/link/platform/windows/link-discover-theme.h")
  endif()
  target_compile_definitions(fordlink-discover PRIVATE
    LINK_THEME_BACKGROUND=0x002C1806
    LINK_THEME_PANEL=0x0044280B
    LINK_THEME_INPUT=0x00271707
    LINK_THEME_TEXT=0x00FBF7F4
    LINK_THEME_MUTED=0x00C9A882
    LINK_THEME_ACCENT=0x00D17A21
    LINK_THEME_ACCENT_TEXT=0x00271707
    "LINK_PRODUCT_FONT_UI=\"Segoe UI\"")
endif()
