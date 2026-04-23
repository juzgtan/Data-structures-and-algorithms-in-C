cmake_minimum_required(VERSION 3.16)
project(Data-structures-and-algorithms-in-C VERSION 1.0.0 LANGUAGES C)

# ============================================================================
# Build Options
# ============================================================================

option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
option(ENABLE_SANITIZERS "Enable address sanitizer" OFF)

# ============================================================================
# C Standard and Compiler Flags
# ============================================================================

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Default compile flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# Coverage flags
if(ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
endif()

# Sanitizer flags
if(ENABLE_SANITIZERS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")
endif()

# ============================================================================
# Directories
# ============================================================================

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# ============================================================================
# Include Directories
# ============================================================================

include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}/utils)
include_directories(${INCLUDE_DIR}/data_structures)

# ============================================================================
# Source Files - Utility Modules
# ============================================================================

set(UTIL_SOURCES
    ${SRC_DIR}/utils/result_code.c
)

# ============================================================================
# Source Files - Data Structures
# ============================================================================

set(DATA_STRUCTURES_SOURCES
    # Array
    # ${SRC_DIR}/data_structures/array/array.c
    
)

# ============================================================================
# Library Target
# ============================================================================

add_library(dsa STATIC ${UTIL_SOURCES} ${DATA_STRUCTURES_SOURCES})

# Set properties for the library
set_target_properties(dsa PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ============================================================================
# Export compile commands (for clangd, etc.)
# ============================================================================

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Subdirectories
# ============================================================================

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ============================================================================
# Print configuration summary
# ============================================================================

message(STATUS "========================================")
message(STATUS "Data-structures-and-algorithms Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Enable coverage: ${ENABLE_COVERAGE}")
message(STATUS "  Enable sanitizers: ${ENABLE_SANITIZERS}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "========================================")

# ============================================================================
# Installation
# ============================================================================

install(TARGETS dsa
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${INCLUDE_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
