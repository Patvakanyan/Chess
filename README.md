# Chess

An object-oriented chess project developed as a C++ term paper. The project
builds the core domain model for a chess game, with pieces, board coordinates,
and piece colors represented as separate types.

## Project Status

The codebase is currently laying the foundation for the game engine:

- `Position` validates board coordinates in the range `0` through `7` and
	supports coordinate arithmetic.
- `Color` identifies the white and black sides.
- `Piece` defines the common interface for chess pieces, including movement
	validation and valid-move generation.
- Piece types are represented by `Pawn`, `Bishop`, `Knight`, `Rook`, `Queen`,
	and `King` classes.
- `Board` and `Square` are reserved for the next stage of the implementation.

Move rules, board state management, captures, check detection, and a playable
interface are planned but are not implemented yet.

## Repository Layout

```text
include/  Public headers and shared domain types
src/      C++ source files
tests/    Test sources
build/    Generated build output (ignored by Git)
```

## Requirements

- A C++ compiler with C++17 support
- GNU Make

## Build and Test

```sh
make       # Build the application when an entry point is available
make test  # Build and run tests when test sources are available
make run   # Run the application when an entry point is available
make clean # Remove generated build output
```

At the current stage, `make` and `make run` report that no application source
has been added, while `make test` reports that no tests have been added.

## Development Roadmap

1. Implement the board and square representations.
2. Complete movement rules for every piece.
3. Add captures, turn handling, and legal-move validation.
4. Add check, checkmate, stalemate, castling, promotion, and en passant.
5. Create automated tests and a command-line game interface.
