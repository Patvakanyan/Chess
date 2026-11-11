#pragma once

#include "chess/Position.hpp"
#include "chess/utils/Color.hpp"
#include "chess/Board.hpp"
#include "chess/utils/EnumPiece.hpp"
#include "chess/Move.hpp"
#include "chess/GameState.hpp"
#include <string>
#include <sstream>

class Move;

namespace ChessAttackUtils
{
	bool isSquareAttacked(const Board &board, const Position &pos, Color color);
	bool isQueenRookAttacked(const Board &board, const Position &pos, Color color);
	bool isQueenBishopAttacked(const Board &board, const Position &pos, Color color);
	bool isKnightAttacked(const Board &board, const Position &pos, Color color);
	bool isPawnAttacked(const Board &board, const Position &pos, Color color);
	bool isKingAttacked(const Board &board, const Position &pos, Color color);
	Position findPiece(const Board &board, Color color, PieceType type);
	bool isInCheck(const Board &board, Color color);
}

namespace ChessMoveUtils
{
	bool isPromotionMove(const Board &board, const Position &from, const Position &to);
	bool isEnPassant(const Board &board, const Position &from, const Position &to,
					 const Move *lastMove);
	bool isCastleKingside(const Board &board, const Position &from, const Position &to);
	bool isCastleQueenside(const Board &board, const Position &from, const Position &to);
	void validateMove(const Board &board, const Position &from, const Position &to,
					  const Piece *pieceToMove, CustomMoveType moveType,
					  PieceType promotionType);
}

namespace ChessGameRules
{
	std::vector<Position> legalMovesFrom(Board &board, const Position &from,
										 const Move *lastMove = nullptr,
										 bool canCastleQueenside = false,
										 bool canCastleKingside = false);
	bool hasLegalMoves(Board &board, Color color, const Move *lastMove = nullptr);
	bool isCheckmate(Board &board, Color color, const Move *lastMove = nullptr);
	bool isStalemate(Board &board, Color color, const Move *lastMove = nullptr);
	bool isFiftyMoveRuleDraw(int halfMoveClock);
}

namespace ChessPieceFactory
{
	std::unique_ptr<Piece> createPiece(PieceType type, Color color);
	std::unique_ptr<Piece> createPromotedPiece(PieceType type, Color color);
}

namespace ChessFenUtils
{
	PieceType charToPieceType(char c);
	void FENToBoard(const std::string &fen, Board &board, GameState &state);
	std::string getCastlingRights(const GameState &state);
	std::string boardToFEN(const Board &board, const GameState &state);
}
