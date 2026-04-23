# Data-structures-and-algorithms-in-C Build Guide

## Prerequisites

- CMake 3.16+
- GCC/Clang with C99 support
- Make (or Ninja)

## Quick Build

```bash
# Clone the repository
cd Data-structures-and-algorithms-in-C

# Make build script executable
chmod +x build.sh

# Build in debug mode with tests
./build.sh -d -t

# Build in release mode
./build.sh -r

# Clean build
./build.sh -c -d -t
```
