#include "chess/utils/ChessUtils.hpp"

namespace ChessFenUtils
{

std::string getCastlingRights(const GameState &state)
{
	std::string rights;
	if (state.castling.whiteKingside) rights += 'K';
	if (state.castling.whiteQueenside) rights += 'Q';
	if (state.castling.blackKingside) rights += 'k';
	if (state.castling.blackQueenside) rights += 'q';

	return rights.empty() ? "-" : rights;
}

PieceType charToPieceType(char c)
{
	switch (std::tolower(c))
	{
	case 'p':
		return PieceType::PAWN;
	case 'r':
		return PieceType::ROOK;
	case 'n':
		return PieceType::KNIGHT;
	case 'b':
		return PieceType::BISHOP;
	case 'q':
		return PieceType::QUEEN;
	case 'k':
		return PieceType::KING;
	default:
		throw std::invalid_argument("Invalid piece character: " + std::string(1, c));
	}
}

void FENToBoard(const std::string &fen, Board &board, GameState &state)
{
	std::istringstream fenStream(fen);
	std::string boardPart, turnPart, castlingPart, enPassantPart, halfMoveClockPart, fullMoveNumberPart;

	if (!(fenStream >> boardPart >> turnPart >> castlingPart >> enPassantPart >> halfMoveClockPart >> fullMoveNumberPart))
	{
		throw std::invalid_argument("Invalid FEN string: " + fen);
	}

	for (size_t i = 0; i < 8; ++i)
	{
		for (size_t j = 0; j < 8; ++j)
		{
			board[i][j].setPiece(nullptr);
		}
	}
	state.castling.whiteKingside = castlingPart.find('K') != std::string::npos;
	state.castling.whiteQueenside = castlingPart.find('Q') != std::string::npos;
	state.castling.blackKingside = castlingPart.find('k') != std::string::npos;
	state.castling.blackQueenside = castlingPart.find('q') != std::string::npos;

	size_t row = 0;
	size_t col = 0;
	for (char c : boardPart)
	{
		if (c == '/')
		{
			++row;
			col = 0;
		}
		else if (std::isdigit(c))
		{
			col += c - '0'; // Skip empty squares
		}
		else
		{
			Color color = std::isupper(c) ? Color::White : Color::Black;
			PieceType type = ChessFenUtils::charToPieceType(c);
			board[row][col].setPiece(ChessPieceFactory::createPiece(type, color));
			++col;
		}
	}

	// Set the current turn based on the FEN string
	state.turn = (turnPart == "w") ? Color::White : Color::Black;

	// Set the half-move clock and full-move number in the move
	// history based on the FEN string
	int halfMoveClock = std::stoi(halfMoveClockPart);
	int fullMoveNumber = std::stoi(fullMoveNumberPart);
	state.halfMoveClock = halfMoveClock;
	state.fullMoveNumber = fullMoveNumber;
	if (enPassantPart == "-")
		state.enPassantTarget.reset();
	else if (enPassantPart.size() == 2 && enPassantPart[0] >= 'a' && enPassantPart[0] <= 'h' &&
			 enPassantPart[1] >= '1' && enPassantPart[1] <= '8')
		state.enPassantTarget = Position(8 - (enPassantPart[1] - '0'), enPassantPart[0] - 'a');
	else
		throw std::invalid_argument("Invalid en passant square in FEN: " + enPassantPart);
}

std::string boardToFEN(const Board &board, const GameState &state)
{
	std::string fen;
	for (size_t i = 0; i < 8; ++i)
	{
		int emptyCount = 0;
		for (size_t j = 0; j < 8; ++j)
		{
			Piece *piece = board[i][j].getPiece();
			if (piece == nullptr)
				++emptyCount;
			else
			{
				if (emptyCount > 0)
				{
					fen += std::to_string(emptyCount);
					emptyCount = 0;
				}
				char pieceChar = pieceTypeToChar(piece->getType());
				fen += piece->getColor() == Color::Black ? static_cast<char>(std::tolower(pieceChar)) : pieceChar;
			}
		}
		if (emptyCount > 0)
			fen += std::to_string(emptyCount);
		if (i != 7)
			fen += '/';
	}
	fen += state.turn == Color::White ? " w " : " b ";
	fen += getCastlingRights(state);
	fen += ' ';
	if (state.enPassantTarget.has_value())
	{
		const Position target = state.enPassantTarget.value();
		fen += static_cast<char>('a' + target.getY());
		fen += std::to_string(8 - target.getX());
	}
	else
		fen += '-';
	fen += ' ';
	fen += std::to_string(state.halfMoveClock);
	fen += ' ';
	fen += std::to_string(state.fullMoveNumber);
	return fen;
}

} // namespace ChessFenUtils
