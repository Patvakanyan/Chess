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
- Qt 6.4 or newer, including the Widgets and Multimedia modules
- GNU Make (required only when CMake needs to build the bundled Stockfish
  fallback)
- A Stockfish executable on `PATH` is optional. If it is not found, CMake
  downloads and builds Stockfish during the build.

## GUI game modes

When the GUI starts, choose one of the following modes:

- **Two Players** - two people play locally on the same board. Stockfish is
  not started in this mode.
- **Play vs Stockfish** - choose White or Black in Settings. Stockfish plays
  the opposite color and moves first when you choose Black. The engine move
  is calculated after each legal human move. Your chosen color is displayed
  at the bottom of the board.

The GUI reports Stockfish startup and move errors in the game status area.
If Stockfish is already installed, CMake uses the executable found through
`PATH`; otherwise the first build downloads and builds it automatically.

Move sounds use the bundled `sound/castle.wav`, a PCM copy of the original
`sound/castle.mp3`, with Qt's sound-effects player. Enable or disable move
sounds from Settings before starting a game.

## Build the GUI

On **Fedora**, **Debian/Ubuntu**, or **Arch Linux**, run this from the
repository root:

```bash
./build.sh
```

The script checks the installed packages, automatically installs missing
dependencies with the distribution's package manager, and builds the project.
This includes the compiler, CMake, GNU Make, Qt Widgets, Qt Multimedia, and
Qt's desktop platform plugins. Package dependencies supply the other system
libraries. It asks for your `sudo` password only if packages need installing;
run the build script as your normal user. Installing packages and downloading
the Stockfish fallback require an internet connection.

Use a distribution with Qt 6.4 or newer in its configured repositories, such
as Ubuntu 24.04+ or Debian 12+. Older repositories will not provide a new
enough Qt. Arch systems should already be up to date before installing packages.

To install dependencies separately or preview the missing packages:

```bash
./scripts/install-dependencies.sh --dry-run
./scripts/install-dependencies.sh
```

To use an existing/custom Qt installation, skip package installation and pass
any needed CMake options:

```bash
./build.sh --skip-deps -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64
```

`BUILD_DIR` changes the build directory and `JOBS` controls parallel compilation
(default: `build` and `2`). For example:

```bash
BUILD_DIR=build/release JOBS=4 ./build.sh -DCMAKE_BUILD_TYPE=Release
```

To build manually after installing the requirements:

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

## Run the network client and server

The root build also creates a console server and client. Start the server in
one terminal:

```bash
./build/server/chess_server
```

Then start a client in each of two other terminals:

```bash
./build/client/chess_client
```

Clients connect to `127.0.0.1:5555` by default. The first client plays White
and waits for the second client, which plays Black. Enter moves such as
`e2e4` when prompted, or `quit` to resign. Closing standard input also resigns
when it is your turn. The server relays moves without checking chess legality.

You can specify an IPv4 address and port:

```bash
./build/client/chess_client 127.0.0.1 5555
```

To build only the client independently:

```bash
cmake -S client -B client/build
cmake --build client/build
./client/build/chess_client
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

The dependency installer also has tests that simulate package managers without
installing system packages (Python 3 is needed only for these tests):

```bash
python3 -m unittest discover -s tests -v
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
