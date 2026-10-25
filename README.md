# Chess GUI

A Qt-based graphical chess application backed by the C++ chess engine in
[`Chess/`](./Chess/).

The repository contains two connected parts:

- **GUI application** - Qt Widgets interface, board rendering, mouse-based
  piece selection, move highlighting, and game status messages.
- **Chess engine** - board state, pieces, move validation, check/checkmate,
  stalemate, en passant, move history, undo, and terminal gameplay.

For the engine-specific design and terminal usage, see the
[Chess engine README](./Chess/Readme.md).

## Requirements

- CMake 3.16 or newer
- A C++17-compatible compiler
- Qt 6.10 or newer
- GNU Make (optional, for the engine-only workflow)

## Build the GUI

Run these commands from the repository root:

```bash
cmake -S . -B build
cmake --build build
```

The GUI executable is created at:

```text
build/appchessGui_2
```

Run it with:

```bash
./build/appchessGui_2
```

## Build and run the engine

The engine can also be built independently:

```bash
cd Chess
make
make run
```

Run the engine tests with:

```bash
cd Chess
make test
```

Run the engine under Valgrind:

```bash
cd Chess
make makeval
```

## CMake tests

The root CMake project includes the engine test target. After configuring
and building:

```bash
ctest --test-dir build --output-on-failure
```

## Project layout

```text
.
├── Chess/             Chess engine and terminal application
├── src/               Qt GUI implementation
├── main.cpp           GUI entry point
├── CMakeLists.txt     Root GUI and engine build configuration
└── README.md          Project documentation
```

## Development notes

The GUI uses `ChessCore` from the engine as a static library. The GUI
controller communicates with the engine and keeps Qt-specific code outside
the engine implementation.

