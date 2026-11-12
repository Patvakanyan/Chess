# Chess GUI

A Qt-based graphical chess application backed by the C++ chess engine in
[`Chess/`](./Chess/).

The repository contains two connected parts:

- **GUI application** - Qt Widgets interface, board rendering, mouse-based
  piece selection, move highlighting, game status messages, and two game
  modes: local two-player and player versus Stockfish.
- **Chess engine** - board state, pieces, move validation, check/checkmate,
  stalemate, castling, en passant, promotion, move history, undo, and terminal
  gameplay. The engine includes a UCI-based Stockfish integration.

For the engine-specific design and terminal usage, see the
[Chess engine README](./Chess/Readme.md).

## Requirements

- CMake 3.16 or newer
- A C++17-compatible compiler
- Qt 6.10 or newer
- GNU Make (required only when CMake needs to build the bundled Stockfish
  fallback)
- A Stockfish executable on `PATH` is optional. If it is not found, CMake
  downloads and builds Stockfish during the build.

## GUI game modes

When the GUI starts, choose one of the following modes:

- **Two Players** - two people play locally on the same board. Stockfish is
  not started in this mode.
- **Play vs Stockfish** - the human player controls White and Stockfish plays
  Black. The engine move is calculated after each legal White move.

The GUI reports Stockfish startup and move errors in the game status area.
If Stockfish is already installed, CMake uses the executable found through
`PATH`; otherwise the first build downloads and builds it automatically.

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

## Build and run the terminal engine

The engine can also be built independently:

```bash
cd Chess
make
make run
```

The terminal workflow starts a game with the human playing White and
Stockfish playing Black. Run the engine tests with:

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
