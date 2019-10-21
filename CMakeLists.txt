CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(LIDEPUB C CXX)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckIncludeFiles)

SET(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
SET(CMAKE_C_STANDARD 11)

# place binaries and libraries according to GNU standards
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

find_package(Threads REQUIRED)

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

ADD_SUBDIRECTORY(test)
