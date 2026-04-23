# 📚 Data Structures and Algorithms (DSA) in C

[![CMake](https://img.shields.io/badge/CMake-3.16+-064F8C?logo=cmake)](https://cmake.org)
[![C](https://img.shields.io/badge/C-99-A8B9CC?logo=c)](<https://en.wikipedia.org/wiki/C_(programming_language)>)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Tests](https://img.shields.io/badge/Tests-Passing-brightgreen)](tests/)

A comprehensive, production-ready implementation of fundamental data structures and algorithms in C, featuring dynamic arrays, linked lists, trees, graphs, hash tables, and more.

## ✨ Features

- **Complete Implementation**: 14+ data structures fully implemented
- **Generic Design**: Works with any data type using `void*` pointers
- **Error Handling**: Unified error codes throughout the library
- **Memory Safe**: Proper memory management with destructor support
- **Well Documented**: Doxygen-style comments for all functions
- **Unit Tested**: Comprehensive test suite for each data structure
- **CMake Build**: Easy integration into any C project

## 🚀 Quick Start

### Prerequisites

- **CMake** 3.16 or higher
- **C compiler** with C99 support (GCC, Clang, MSVC)
- **Make** (or Ninja)

### Building the Library

```bash
# Clone the repository
git clone https://github.com/yourusername/Data-structures-and-algorithms-in-C.git
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
