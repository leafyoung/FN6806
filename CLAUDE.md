# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

FN6806 is a C++ educational code repository for the course "OOP II - C++ Programming". It contains 30+ self-contained modules, each in a numbered directory demonstrating specific C++ concepts.

## Build & Run

Each module is compiled independently. There is no top-level build system. Navigate into a module directory and compile with g++:

```bash
# Basic compilation
g++ -std=c++17 -o main main.cpp *.cpp

# For C++20 modules
g++ -std=c++20 -o main main.cpp *.cpp

# With threading (required for 54-thread, 55-thread-atomic, 71-multithread_mc_pi)
g++ -std=c++17 -pthread -o main main.cpp *.cpp

# Run
./main
```

Some modules (e.g., `52-mc_gbm`) write output to an `output/` subdirectory — create it before running if needed: `mkdir -p output`.

## Module Structure

Each module directory follows a consistent pattern:
- `main.cpp` — entry point; calls test/demo functions defined in companion files
- `*.h` / `*.cpp` pairs — class/function definitions and implementations included by main.cpp

Multi-file modules (e.g., `54-thread`, `60-exception`) split demonstrations into named files like `test_thread.cpp`, `01_test_vector.cpp`, etc. The `main.cpp` declares and calls those functions.

## Module Index

| Directory | Topic |
|-----------|-------|
| `10-class_rectangle`, `12-class_point` | Basic class design |
| `20-cards`, `22-operator` | Operator overloading |
| `30-36` | Function and class templates |
| `38-raw_pointer`, `3a-unique_ptr`, `3c-shared_ptr`, `3e-weak_ptr` | Pointer ownership |
| `40-42`, `45-49`, `4a-4c` | Inheritance, polymorphism, CRTP, `std::variant` |
| `50-valarray` | `std::valarray` |
| `52-mc_gbm` | Monte Carlo Geometric Brownian Motion (single/multi-threaded, Eigen) |
| `54-thread`, `55-thread-atomic` | `std::thread`, `std::mutex`, `std::future`, `std::atomic` |
| `60-exception` | Exception handling patterns |
| `70-chrono`, `71-multithread_mc_pi` | Timing, multi-threaded Monte Carlo |
| `80-hash` | Hash functions |

## Key Architectural Patterns

- **No external dependencies** except standard library. Module `52-mc_gbm` references Eigen headers (optional path); if Eigen is unavailable, exclude `gbm_multi_thread_eigen.*` from compilation.
- **C++ standards used**: C++11 through C++20 depending on the module. Most modules target C++17.
- **Replit workflow**: The repo was developed on Replit using sparse checkout (see `replit_setup.md`) to work on one module at a time. Each module is self-contained for this reason.
- **Output files**: `52-mc_gbm` writes CSV files to `output/` subdirectory.

## Adding a New Module

1. Create a new numbered directory following the naming convention (hex-like prefix + descriptive name).
2. Add `main.cpp` as the entry point.
3. Split topic demonstrations into `*_topic.cpp` / `*_topic.h` pairs if the module covers multiple sub-topics.
4. Keep all includes relative (no install-time paths needed).
