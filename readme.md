

## Project Build Instructions

This project uses a Makefile-based build system to compile the main application and test suites.

### Prerequisites

Before building, ensure you have the following dependencies installed:

- Required Libraries
    - GLFW: Window and input management
    - GLEW: OpenGL Extension Wrangler Library
    - OpenGL: Graphics library
    - GLU: OpenGL Utilities

- Development Tools
    - g++ (GNU C++ Compiler)
    - make (GNU Make)
    - valgrind (for memory checking and profiling)

### Installation on Ubuntu/Debian
```bash
sudo apt update
sudo apt install g++ make valgrind libglfw3-dev libglew-dev libglu1-mesa-dev libgl1-mesa-dev
```

### Build Commands
Build Main Application

```bash
make
```
Build and Run Main Application
```bash
make run
```
Memory Leak Checking
```bash
make leak_check
```
Removes all generated files including object files, executables, and profiling reports.
```bash
make clean
```