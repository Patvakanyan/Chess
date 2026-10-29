#include "chess/utils/ChessUtils.hpp"
#include <memory>

Position ChessUtils::findKing(const Board &board, Color color)
{
	for (size_t i = 0; i < 8; ++i)
	{
		for (size_t j = 0; j < 8; j++)
		{
			Piece *tmp = board[i][j].getPiece();
			if (dynamic_cast<King *>(tmp) && tmp->getColor() == color)
				return Position(i, j);
		}
	}
	return Position(-1, -1);
}

bool ChessUtils::isQueenBishopAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<Position> directions = {
		Position(-1, -1), Position(-1, 1),
		Position(1, -1), Position(1, 1)};

	for (auto dir : directions)
	{
		int currentX = pos.getX() + dir.getX();
		int currentY = pos.getY() + dir.getY();
		while (currentX >= 0 && currentX < 8 && currentY >= 0 && currentY < 8)
		{
			const Square &square = board[currentX][currentY];
			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece->getColor() == color && (dynamic_cast<Queen *>(piece) || dynamic_cast<Bishop *>(piece)))
				{
					return true;
				}
				break;
			}
			currentX += dir.getX();
			currentY += dir.getY();
		}
	}
	return false;
}

bool ChessUtils::isQueenRookAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<Position> directions = {
		Position(-1, 0), Position(1, 0),
		Position(0, -1), Position(0, 1)};

	for (auto dir : directions)
	{
		int currentX = pos.getX() + dir.getX();
		int currentY = pos.getY() + dir.getY();
		while (currentX >= 0 && currentX < 8 && currentY >= 0 && currentY < 8)
		{
			const Square &square = board[currentX][currentY];

			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece->getColor() == color && (dynamic_cast<Queen *>(piece) || dynamic_cast<Rook *>(piece)))
				{
					return true;
				}
				break;
			}
			currentX += dir.getX();
			currentY += dir.getY();
		}
	}
	return false;
}

bool ChessUtils::isKnightAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<Position> knightMoves = {
		Position(-2, -1), Position(-2, 1),
		Position(-1, -2), Position(-1, 2),
		Position(1, -2), Position(1, 2),
		Position(2, -1), Position(2, 1)};

	for (const auto &move : knightMoves)
	{
		int newX = pos.getX() + move.getX();
		int newY = pos.getY() + move.getY();
		if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
		{
			const Square &square = board[newX][newY];
			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece && piece->getColor() == color && dynamic_cast<Knight *>(piece))
					return true;
			}
		}
	}
	return false;
}

bool ChessUtils::isPawnAttacked(const Board &board, const Position &pos, Color color)
{
	// To find attackers of this square, inspect the row opposite the pawn's
	// movement direction.
	int direction = (color == Color::White) ? 1 : -1;
	std::vector<Position> pawnAttacks = {
		Position(direction, -1),
		Position(direction, 1)};

	for (const auto &attack : pawnAttacks)
	{
		int newX = pos.getX() + attack.getX();
		int newY = pos.getY() + attack.getY();
		if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
		{
			const Square &square = board[newX][newY];
			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece->getColor() == color && dynamic_cast<Pawn *>(piece))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool ChessUtils::isKingAttacked(const Board &board, const Position &pos, Color color)
{
	const std::vector<Position> kingMoves = {
		Position(-1, -1), Position(-1, 0), Position(-1, 1),
		Position(0, -1), Position(0, 1),
		Position(1, -1), Position(1, 0), Position(1, 1)};

	for (const auto &move : kingMoves)
	{
		int newX = pos.getX() + move.getX();
		int newY = pos.getY() + move.getY();
		if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
		{
			const Square &square = board[newX][newY];
			if (!square.isEmpty())
			{
				Piece *piece = square.getPiece();
				if (piece->getColor() == color && dynamic_cast<King *>(piece))
					return true;
			}
		}
	}
	return false;
}

bool ChessUtils::isSquareAttacked(const Board &board, const Position &pos, Color color)
{
	return isQueenRookAttacked(board, pos, color) ||
		   isQueenBishopAttacked(board, pos, color) ||
		   isKnightAttacked(board, pos, color) ||
		   isPawnAttacked(board, pos, color) ||
		   isKingAttacked(board, pos, color);
}
bool ChessUtils::isInCheck(const Board &board, Color color)
{
	Position kingPos = findKing(board, color);
	if (kingPos.getX() == -1)
	{
		throw std::runtime_error("King not found on the board.");
	}
	Color opponentColor = (color == Color::White) ? Color::Black : Color::White;
	return isSquareAttacked(board, kingPos, opponentColor);
}

std::string ChessUtils::getCastlingRights(const Board &board)
{
	std::string rights;

	// Check White's castling rights
	Piece *whiteKing = board[7][4].getPiece();
	Piece *whiteKingsideRook = board[7][7].getPiece();
	Piece *whiteQueensideRook = board[7][0].getPiece();

	if (whiteKing && whiteKing->getType() == PieceType::KING && whiteKing->getHasMoved() == 0)
	{
		if (whiteKingsideRook && whiteKingsideRook->getType() == PieceType::ROOK && whiteKingsideRook->getHasMoved() == 0)
		{
			rights += 'K';
		}
		if (whiteQueensideRook && whiteQueensideRook->getType() == PieceType::ROOK && whiteQueensideRook->getHasMoved() == 0)
		{
			rights += 'Q';
		}
	}

	// Check Black's castling rights
	Piece *blackKing = board[0][4].getPiece();
	Piece *blackKingsideRook = board[0][7].getPiece();
	Piece *blackQueensideRook = board[0][0].getPiece();

	if (blackKing && blackKing->getType() == PieceType::KING && blackKing->getHasMoved() == 0)
	{
		if (blackKingsideRook && blackKingsideRook->getType() == PieceType::ROOK && blackKingsideRook->getHasMoved() == 0)
		{
			rights += 'k';
		}
		if (blackQueensideRook && blackQueensideRook->getType() == PieceType::ROOK && blackQueensideRook->getHasMoved() == 0)
		{
			rights += 'q';
		}
	}

	return rights.empty() ? "- -" : rights;
}

namespace
{
	std::unique_ptr<Piece> createPiece(PieceType type, Color color)
	{
		switch (type)
		{
		case PieceType::PAWN:
			return std::make_unique<Pawn>(color);
		case PieceType::ROOK:
			return std::make_unique<Rook>(color);
		case PieceType::KNIGHT:
			return std::make_unique<Knight>(color);
		case PieceType::BISHOP:
			return std::make_unique<Bishop>(color);
		case PieceType::QUEEN:
			return std::make_unique<Queen>(color);
		case PieceType::KING:
			return std::make_unique<King>(color);
		default:
			return nullptr;
		}
	}
}

bool ChessUtils::hasLegalMoves(Board &board, Color color, const Move *lastMove)
{
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			Piece *piece = board[i][j].getPiece();
			if (piece == nullptr)
				continue;

			if (piece->getColor() != color)
				continue;
			Position currentPos = Position(i, j);
			std::vector<Position> validMoves = piece->getValidMoves(currentPos, board);
			for (const Position &newPos : validMoves)
			{
				Move candidate(currentPos, newPos);
				try
				{
					candidate.makeMove(board, lastMove);
				}
				catch (const InvalidMoveException &)
				{
					continue;
				}

				const bool legal = !isInCheck(board, color);
				const Color capturedColor =
					piece->getColor() == Color::White ? Color::Black : Color::White;
				std::unique_ptr<Piece> movingPiece =
					board[newPos.getX()][newPos.getY()].releasePiece();
				movingPiece->subtractHasMoved();
				board[currentPos.getX()][currentPos.getY()].setPiece(std::move(movingPiece));

				if (candidate.getCapturedPieceType() != PieceType::NONE)
				{
					std::unique_ptr<Piece> capturedPiece =
						createPiece(candidate.getCapturedPieceType(), capturedColor);
					if (capturedPiece == nullptr)
						throw std::runtime_error("Failed to restore captured piece.");
					for (size_t count = 0; count < candidate.getCapturedPieceHasMoved(); ++count)
						capturedPiece->addHasMoved();
					const Position &capturedPosition = candidate.getCapturedPosition();
					board[capturedPosition.getX()][capturedPosition.getY()]
						.setPiece(std::move(capturedPiece));
				}

				if (legal)
					return true;
			}
		}
	}
	return false;
}

bool ChessUtils::isCheckmate(Board &board, Color color, const Move *lastMove)
{
	return isInCheck(board, color) && !hasLegalMoves(board, color, lastMove);
}

bool ChessUtils::isStalemate(Board &board, Color color, const Move *lastMove)
{
	return !isInCheck(board, color) && !hasLegalMoves(board, color, lastMove);
}

bool ChessUtils::isFiftyMoveRuleDraw(int halfMoveClock)
{
	return halfMoveClock >= 100;
}

void ChessUtils::FENToBoard(const std::string &fen, Board &board, Color &currentTurn, MoveHistory &moveHistory)
{
	std::cout << "Parsing FEN: " << fen << std::endl;
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
			PieceType type;
			switch (std::tolower(c))
			{
			case 'p':
				type = PieceType::PAWN;
				break;
			case 'r':
				type = PieceType::ROOK;
				break;
			case 'n':
				type = PieceType::KNIGHT;
				break;
			case 'b':
				type = PieceType::BISHOP;
				break;
			case 'q':
				type = PieceType::QUEEN;
				break;
			case 'k':
				type = PieceType::KING;
				break;
			default:
				throw std::invalid_argument("Invalid piece character in FEN: " + std::string(1, c));
			}
			board[row][col].setPiece(createPiece(type, color));
			++col;
		}
	}

	// Set the current turn based on the FEN string
	currentTurn = (turnPart == "w") ? Color::White : Color::Black;

	// Set the half-move clock and full-move number in the move
	// history based on the FEN string
	int halfMoveClock = std::stoi(halfMoveClockPart);
	int fullMoveNumber = std::stoi(fullMoveNumberPart);
	moveHistory.setHalfMoveClock(halfMoveClock);
	moveHistory.setFullMoveNumber(fullMoveNumber);
}
