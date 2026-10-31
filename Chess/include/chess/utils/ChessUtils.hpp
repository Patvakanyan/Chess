#pragma once

#include "chess/Position.hpp"
#include "chess/utils/Color.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"
#include "chess/MoveHistory.hpp"
#include "chess/Move.hpp"
#include <string>
#include <sstream>

class Move;
class MoveHistory;

class ChessUtils
{
public:
	ChessUtils() = delete;
	~ChessUtils() = delete;

	static bool isSquareAttacked(const Board &board, const Position &pos, Color color);
	static bool isQueenRookAttacked(const Board &board, const Position &pos, Color color);
	static bool isQueenBishopAttacked(const Board &board, const Position &pos, Color color);
	static bool isKnightAttacked(const Board &board, const Position &pos, Color color);
	static bool isPawnAttacked(const Board &board, const Position &pos, Color color);
	static bool isKingAttacked(const Board &board, const Position &pos, Color color);

	static bool hasLegalMoves(Board &board, Color color, const Move *lastMove = nullptr);
	static Position findKing(const Board &board, Color color);

	static PieceType charToPieceType(char c);
	static void FENToBoard(const std::string &fen, Board &board, Color &currentTurn, MoveHistory &moveHistory);
	static bool isInCheck(const Board &board, Color color);

	static std::unique_ptr<Piece> createPiece(PieceType type, Color color);
	static std::unique_ptr<Piece> createPromotedPiece(PieceType type, Color color);
	static std::string getCastlingRights(const Board &board);
	static bool isCheckmate(Board &board, Color color, const Move *lastMove = nullptr);
	static bool isStalemate(Board &board, Color color, const Move *lastMove = nullptr);
	static bool isFiftyMoveRuleDraw(int halfMoveClock);
};
