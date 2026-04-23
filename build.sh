#!/bin/bash

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Default values
BUILD_TYPE="Debug"
BUILD_TESTS="ON"
CLEAN_BUILD=false
RUN_TESTS=false
COVERAGE=false
SANITIZERS=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -r|--release)
            BUILD_TYPE="Release"
            BUILD_TESTS="OFF"
            shift
            ;;
        -d|--debug)
            BUILD_TYPE="Debug"
            BUILD_TESTS="ON"
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -t|--test)
            RUN_TESTS=true
            shift
            ;;
        --coverage)
            COVERAGE=true
            BUILD_TYPE="Debug"
            shift
            ;;
        --sanitize)
            SANITIZERS=true
            BUILD_TYPE="Debug"
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  -r, --release    Build in release mode"
            echo "  -d, --debug      Build in debug mode (default)"
            echo "  -c, --clean      Clean build directory before building"
            echo "  -t, --test       Run tests after building"
            echo "  --coverage       Enable code coverage"
            echo "  --sanitize       Enable address sanitizer"
            echo "  -h, --help       Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

# Create build directory
BUILD_DIR="build"
if [ "$CLEAN_BUILD" = true ] && [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake
echo -e "${YELLOW}Configuring CMake...${NC}"
echo -e "  Build type: $BUILD_TYPE"
echo -e "  Build tests: $BUILD_TESTS"
echo -e "  Coverage: $COVERAGE"
echo -e "  Sanitizers: $SANITIZERS"

cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DBUILD_TESTS=$BUILD_TESTS \
    -DENABLE_COVERAGE=$COVERAGE \
    -DENABLE_SANITIZERS=$SANITIZERS

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

# Build
echo -e "${YELLOW}Building...${NC}"
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}Build successful!${NC}"

# Run tests if requested
if [ "$RUN_TESTS" = true ]; then
    echo -e "${YELLOW}Running tests...${NC}"
    ctest --output-on-failure
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}All tests passed!${NC}"
    else
        echo -e "${RED}Some tests failed!${NC}"
        exit 1
    fi
fi

# Generate coverage report if enabled
if [ "$COVERAGE" = true ]; then
    echo -e "${YELLOW}Generating coverage report...${NC}"
    make coverage 2>/dev/null || echo -e "${YELLOW}Coverage target not available${NC}"
fi

echo -e "${GREEN}Done!${NC}"