cmake_minimum_required(VERSION 3.14)
project("GaBox2d" VERSION 2.0)

# Enable code coverage if requested
option(GABOX2D_ENABLE_COVERAGE "Enable code coverage for tests" OFF)
set(GABOX2D_ENABLE_COVERAGE OFF CACHE BOOL "Enable code coverage for tests" FORCE)
message(STATUS "Code coverage: ${GABOX2D_ENABLE_COVERAGE}")

include(cmake/ProjectSettings.cmake)
include(cmake/Dependencies.cmake)
include(cmake/Core.cmake)
include(cmake/MainTarget.cmake)
include(cmake/Testing.cmake)
