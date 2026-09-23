# Chess Engine

A C++ chess project built around an object-oriented board model, piece logic, and a terminal-based game flow. The codebase includes board state management, chess-piece rules, legal-move checks, and a small terminal user interface.

## Project status

This project is under active development and currently includes:

- a board and square model
- chess-piece abstractions and concrete piece types
- position validation and coordinate handling
- color tracking for both sides
- move history and game state management
- legal-move and check detection helpers
- a terminal-based game loop

## Repository layout

```text
include/chess/    Public headers for the game model
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
make        # build the chess application
make run    # build and run the program
make test   # build and run the test suite
make clean  # remove generated build files
make re     # clean and rebuild everything
```

## Running the game

The main entry point is in `src/main.cpp` and launches a terminal-based chess game through the `Game` and `Terminal` classes.

## Testing

The project includes a basic test program in `tests/chessUtils_test.cpp` covering logic such as:

- check detection
- rook/bishop/queen/knight/pawn/king attack checks
- blocked attacks
- checkmate detection
- stalemate detection

## Notes

The project is structured as a chess engine and game scaffolding rather than a full production-ready chess application. Features such as full move legality, castling, en passant, promotion, and some edge-case rule handling may still be incomplete depending on the current branch state.
