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

# Set Cpp11
set (CMAKE_CXX_STANDARD 11)

# Set project title
project(samp-plugin-streamer)

# Plattform specified definitions
if (WIN32)
	add_definitions(
		-DBOOST_ALL_NO_LIB
		-DBOOST_CHRONO_HEADER_ONLY
		-DNDEBUG
		-DNOMINMAX
		-DSAMPGDK_AMALGAMATION
		-DSAMPGDK_CPP_WRAPPERS
		-DWIN32
	)
else()
	add_definitions(
		-DBOOST_CHRONO_HEADER_ONLY
		-DLINUX
		-DNDEBUG
		-DSAMPGDK_AMALGAMATION
		-DSAMPGDK_CPP_WRAPPERS
	)
endif()

# Base path constants
set(SAMP_STREAMER_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SAMP_STREAMER_BIN_ROOT ${CMAKE_CURRENT_BINARY_DIR})

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SAMP_STREAMER_BIN_ROOT}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SAMP_STREAMER_BIN_ROOT}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SAMP_STREAMER_BIN_ROOT}/bin)

# Externals
set(SAMP_STREAMER_EXTERNAL ${SAMP_STREAMER_ROOT}/external)

# Module
set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${SAMP_STREAMER_EXTERNAL}/sampcmake
)

# Set SDK Path
SET(SAMPSDK_PATH ${SAMP_STREAMER_EXTERNAL}/sampsdk)
SET(SAMPGDK_PATH ${SAMP_STREAMER_EXTERNAL}/sampgdk)

# Add src
add_subdirectory(src)
