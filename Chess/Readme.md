# Chess Engine

A C++ Chess project built around an object-oriented board model, piece logic, and a terminal-based game flow. The codebase includes board state management, Chess-piece rules, legal-move checks, and a small terminal user interface.

## Project status

This project is under active development and currently includes:

- a board and square model
- Chess-piece abstractions and concrete piece types
- position validation and coordinate handling
- color tracking for both sides
- move history and game state management
- legal-move and check detection helpers
- a terminal-based game loop

## Repository layout

```text
include/Chess/    Public headers for the game model
src/              C++ implementation files
src/pieces/       Piece implementations
src/exceptions/  Chess exception classes
tests/            Automated check tests
obj/              Generated object files
```

## Requirements

- C++17-compatible compiler
- GNU Make

## Build and run

```bash
make        # build the Chess application
make run    # build and run the program
make test   # build and run the test suite
make clean  # remove generated build files
make re     # clean and rebuild everything
```

## Running the game

The main entry point is in `src/Main.cpp` and launches a terminal-based Chess game through the `Game` and `Terminal` classes.

## Testing

The project includes a basic test program in `tests/ChessUtils_test.cpp` covering logic such as:

- check detection
- rook/bishop/queen/knight/pawn/king attack checks
- blocked attacks
- checkmate detection
- stalemate detection

## Notes

The project is structured as a Chess engine and game scaffolding rather than a full production-ready Chess application. Features such as full move legality, castling, en passant, promotion, and some edge-case rule handling may still be incomplete depending on the current branch state.
