CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(LIDEPUB C CXX)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckIncludeFiles)

SET(CMAKE_EXPORT_COMPILE_COMMANDS on)
# SET(CMAKE_VERBOSE_MAKEFILE on)
SET(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-fprofile-arcs -ftest-coverage")
SET(CMAKE_CXX_STANDARD 17)

# place binaries and libraries according to GNU standards
INCLUDE(GNUInstallDirs)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

FIND_PACKAGE(Threads REQUIRED)

ADD_CUSTOM_TARGET(third_parties)
INCLUDE(ExternalProject)
ExternalProject_Add(
  google_gtest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
  PREFIX ${PROJECT_SOURCE_DIR}/third/gtest
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/third/gtest/build -DBUILD_SHARED_LIBS=OFF
  )
ADD_DEPENDENCIES(third_parties google_gtest)

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/third/gtest/build/include
  )
LINK_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/third/gtest/build/lib
  )

# Language C
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/actors/common
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/actors/basic
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/actors/communication/channel_estimation
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/actors/communication/common
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/actors/dsp
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/actors/hep
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/actors/imaging/gaussian_filtering
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/edges/basic
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/graph/common
    ${PROJECT_SOURCE_DIR}/lang/c/src/gems/graph/basic
    ${PROJECT_SOURCE_DIR}/lang/c/src/tools/runtime
    )

ADD_SUBDIRECTORY(lang/c/src/gems/actors/basic)
ADD_SUBDIRECTORY(lang/c/src/gems/actors/communication/channel_estimation)
ADD_SUBDIRECTORY(lang/c/src/gems/actors/communication/common)
ADD_SUBDIRECTORY(lang/c/src/gems/actors/dsp)
ADD_SUBDIRECTORY(lang/c/src/gems/actors/hep)
ADD_SUBDIRECTORY(lang/c/src/gems/actors/imaging/gaussian_filtering)
ADD_SUBDIRECTORY(lang/c/src/gems/edges/basic)
ADD_SUBDIRECTORY(lang/c/src/gems/graph/basic)
ADD_SUBDIRECTORY(lang/c/src/tools/runtime)

# Language OCL
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/lang/ocl/src/gems/actors/common
    ${PROJECT_SOURCE_DIR}/lang/ocl/src/gems/actors/basic
    ${PROJECT_SOURCE_DIR}/lang/ocl/src/gems/edges/basic
    ${PROJECT_SOURCE_DIR}/lang/ocl/src/gems/graph/basic
    ${PROJECT_SOURCE_DIR}/lang/ocl/src/gems/graph/common
    ${PROJECT_SOURCE_DIR}/lang/ocl/src/tools/runtime
    )
ADD_SUBDIRECTORY(lang/ocl/src/gems/actors/basic)
ADD_SUBDIRECTORY(lang/ocl/src/gems/edges/basic)
ADD_SUBDIRECTORY(lang/ocl/src/gems/graph/basic)
ADD_SUBDIRECTORY(lang/ocl/src/tools/runtime)


ENABLE_TESTING()

ADD_SUBDIRECTORY(test)
