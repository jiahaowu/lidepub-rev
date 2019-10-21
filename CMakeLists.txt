CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(LIDEPUB C CXX)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckIncludeFiles)

SET(CMAKE_EXPORT_COMPILE_COMMANDS on)
option(CODE_COVERAGE "Enable coverage reporting" ON)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif(CODE_COVERAGE)
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
