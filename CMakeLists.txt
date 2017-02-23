# CMake required version
cmake_minimum_required(VERSION 3.1)

# "Only interpret if() arguments as variables or keywords when unquoted."
cmake_policy(SET CMP0054 NEW)

# Set cmake build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Options
option(SAMP_STREAMER_USE_FOREIGN_EXTERNAL "Use foreign externals" OFF)
option(SAMP_STREAMER_BUILD_DOCS "Build Doxygen documentation" ON)

# Set project title
project(samp-plugin-streamer)

# Base path constants
set(SAMP_STREAMER_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SAMP_STREAMER_BIN_ROOT ${CMAKE_CURRENT_BINARY_DIR})

# Externals
set(SAMP_STREAMER_EXTERNAL ${SAMP_STREAMER_ROOT}/external)
set(SAMP_STREAMER_EXTERNAL_BIN ${SAMP_STREAMER_BIN_ROOT}/external)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SAMP_STREAMER_BIN_ROOT}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SAMP_STREAMER_BIN_ROOT}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SAMP_STREAMER_BIN_ROOT}/bin)

# Module
set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${SAMP_STREAMER_ROOT}/build/cmake
)

# Include externals
if (NOT SAMP_STREAMER_USE_FOREIGN_EXTERNAL)
	include(StreamerExternal)
endif()

# Include other
include(StreamerDefines)
include(StreamerFunctions)

# Add src
add_subdirectory(src)
