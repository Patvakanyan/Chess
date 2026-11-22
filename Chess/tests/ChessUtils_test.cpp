#include "chess/utils/ChessUtils.hpp"
#include "chess/Game.hpp"
#include <vector>
#include <algorithm>

namespace
{
	const Position kingPosition(4, 4);

	bool isCheck(Color kingColor, std::unique_ptr<Piece> attacker, const Position &attackerPosition)
	{
		Board board;
		board.initializeBoard();
		board.placePiece(std::make_unique<King>(kingColor), kingPosition);
		board.placePiece(std::move(attacker), attackerPosition);
		return ChessAttackUtils::isInCheck(board, kingColor);
	}

	bool blockedRookIsNotCheck()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(std::make_unique<King>(Color::White), kingPosition);
		board.placePiece(std::make_unique<Rook>(Color::Black), Position(0, 4));
		board.placePiece(std::make_unique<Pawn>(Color::White), Position(2, 4));
		return !ChessAttackUtils::isInCheck(board, Color::White);
	}

	bool protectedQueenCheckmatesKing()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(std::make_unique<King>(Color::Black), Position(0, 0));
		board.placePiece(std::make_unique<Queen>(Color::White), Position(1, 1));
		board.placePiece(std::make_unique<King>(Color::White), Position(2, 2));
		return ChessAttackUtils::isInCheck(board, Color::Black) &&
			   !ChessGameRules::hasLegalMoves(board, Color::Black) &&
			   ChessGameRules::isCheckmate(board, Color::Black);
	}

	bool checkWithKingEscapeIsNotCheckmate()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(std::make_unique<King>(Color::Black), Position(0, 0));
		board.placePiece(std::make_unique<Queen>(Color::White), Position(1, 1));
		board.placePiece(std::make_unique<King>(Color::White), Position(3, 3));
		return ChessAttackUtils::isInCheck(board, Color::Black) &&
			   !ChessGameRules::isCheckmate(board, Color::Black);
	}

	bool stalemateIsNotCheckmate()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(std::make_unique<King>(Color::Black), Position(0, 0));
		board.placePiece(std::make_unique<Queen>(Color::White), Position(1, 2));
		board.placePiece(std::make_unique<King>(Color::White), Position(2, 2));
		return ChessGameRules::isStalemate(board, Color::Black) &&
			   !ChessGameRules::isCheckmate(board, Color::Black);
	}

	bool ordinaryPositionIsNeitherCheckmateNorStalemate()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(std::make_unique<King>(Color::White), Position(4, 4));
		board.placePiece(std::make_unique<King>(Color::Black), Position(0, 0));
		return !ChessAttackUtils::isInCheck(board, Color::White) &&
			   ChessGameRules::hasLegalMoves(board, Color::White) &&
			   !ChessGameRules::isCheckmate(board, Color::White) &&
			   !ChessGameRules::isStalemate(board, Color::White);
	}

	bool fiftyMoveRuleUses100HalfMoves()
	{
		Board board;
		board.initializeBoard();
		board.placePiece(std::make_unique<King>(Color::White), Position(4, 4));
		board.placePiece(std::make_unique<King>(Color::Black), Position(0, 0));
		return !ChessGameRules::isStalemate(board, Color::White) &&
			   !ChessGameRules::isFiftyMoveRuleDraw(99) &&
			   ChessGameRules::isFiftyMoveRuleDraw(100);
	}

	bool undoRestoresCapturedPieceMovementState()
	{
		Board board;
		board.placePiece(std::make_unique<King>(Color::White), Position(7, 7));
		board.placePiece(std::make_unique<King>(Color::Black), Position(7, 0));
		board.placePiece(std::make_unique<Rook>(Color::White), Position(0, 0));
		board.placePiece(std::make_unique<Rook>(Color::Black), Position(0, 2));

		std::vector<Move> history;
		Move whiteRookMove(Position(0, 0), Position(0, 1));
		whiteRookMove.makeMove(board);
		history.push_back(whiteRookMove);

		Move blackRookCapture(Position(0, 2), Position(0, 1));
		blackRookCapture.makeMove(board);
		history.push_back(blackRookCapture);

		history.back().undoMove(board);
		history.pop_back();
		return board[0][1].getPiece()->getType() == PieceType::ROOK;
	}

	bool kingCannotBeCaptured()
	{
		Board board;
		board.placePiece(std::make_unique<King>(Color::White), Position(7, 7));
		board.placePiece(std::make_unique<King>(Color::Black), Position(0, 0));
		board.placePiece(std::make_unique<Queen>(Color::White), Position(1, 1));

		try
		{
			Move captureKing(Position(1, 1), Position(0, 0));
			captureKing.makeMove(board);
		}
		catch (const InvalidMoveException &)
		{
			return true;
		}
		return false;
	}

	bool pawnDoubleStepRequiresStartingRank()
	{
		Board board;
		Pawn whitePawn(Color::White);
		Pawn blackPawn(Color::Black);

		return !whitePawn.isValidMove(Position(4, 0), Position(2, 0), board) &&
			   !blackPawn.isValidMove(Position(3, 1), Position(5, 1), board) &&
			   whitePawn.isValidMove(Position(6, 0), Position(4, 0), board) &&
			   blackPawn.isValidMove(Position(1, 1), Position(3, 1), board);
	}

	bool aNewGameClearsMoveHistory()
	{
		Game game;
		game.startGame();
		game.makeMove(Position(7, 1), Position(5, 2));
		game.startGame();

		bool historyCleared = false;
		try
		{
			game.undoMove();
		}
		catch (const InvalidMoveException &)
		{
			historyCleared = true;
		}
		return historyCleared && game.getHalfMoveClock() == 0 && game.getCurrentTurn() == Color::White;
	}

	bool loadingFenClearsMoveHistory()
	{
		Game game;
		game.startGame();
		game.makeMove(Position(7, 1), Position(5, 2));
		game.FENToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

		try
		{
			game.undoMove();
		}
		catch (const InvalidMoveException &)
		{
			return true;
		}
		return false;
	}

	const std::vector<PieceType> promotionChoices = {
		PieceType::QUEEN, PieceType::ROOK, PieceType::BISHOP, PieceType::KNIGHT};

	bool rejectedPromotionsPreservePosition()
	{
		for (Color color : {Color::White, Color::Black})
		{
			for (const std::string target : {"a", "b"})
			{
				for (PieceType choice : promotionChoices)
				{
					Game game;
					game.FENToBoard(color == Color::White
						? "1r2k3/P7/8/8/8/8/2n5/4K3 w - - 7 12"
						: "4k3/2N5/8/8/8/8/p7/1R2K3 b - - 7 12");
					const Position from(color == Color::White ? "a7" : "a2");
					const Position to(target + (color == Color::White ? "8" : "1"));
					const std::string before = ChessFenUtils::boardToFEN(game.getBoard(), GameState());
					const std::string fen = game.getFEN();
					const Piece *pawn = game.getBoard()[from.getX()][from.getY()].getPiece();
					bool rejected = false;
					try
					{
						game.makeMove(from, to, choice);
					}
					catch (const InvalidMoveException &)
					{
						rejected = true;
					}
					if (!rejected ||
						ChessFenUtils::boardToFEN(game.getBoard(), GameState()) != before ||
						game.getBoard()[from.getX()][from.getY()].getPiece() != pawn ||
						game.getFEN() != fen || game.getCurrentTurn() != color ||
						game.getHalfMoveClock() != 7 || game.getLastMove() != nullptr ||
						game.getResult() != ChessGameResult::Ongoing || game.isGameOver())
						return false;
				}
			}
		}
		return true;
	}

	bool promotionQueriesPreservePosition()
	{
		for (Color color : {Color::White, Color::Black})
		{
			Game game;
			game.FENToBoard(color == Color::White
				? "1r2k3/P7/8/8/8/8/2n5/4K3 w - - 7 12"
				: "4k3/2N5/8/8/8/8/p7/1R2K3 b - - 7 12");
			const Position from(color == Color::White ? "a7" : "a2");
			const std::string before = ChessFenUtils::boardToFEN(game.getBoard(), GameState());
			for (int attempt = 0; attempt < 2; ++attempt)
			{
				if (!game.legalMovesFrom(from).empty() ||
					ChessFenUtils::boardToFEN(game.getBoard(), GameState()) != before)
					return false;
			}
			if (ChessGameRules::isCheckmate(game.getBoard(), color) ||
				ChessFenUtils::boardToFEN(game.getBoard(), GameState()) != before)
				return false;
		}
		return true;
	}

	bool givingCheckDoesNotPromoteOpponentPawn()
	{
		Game game;
		game.FENToBoard("1rbqkbnr/Pp2pppp/3P4/8/3n4/8/P2Q1PPP/RNB1KBNR b - - 0 9");
		game.makeMove(Position("d4"), Position("c2"));
		const Piece *pawn = game.getBoard()[1][0].getPiece();
		return pawn != nullptr && pawn->getType() == PieceType::PAWN &&
			   pawn->getColor() == Color::White &&
			   game.getCurrentTurn() == Color::White &&
			   ChessAttackUtils::isInCheck(game.getBoard(), Color::White) &&
			   game.legalMovesFrom(Position("a7")).empty() &&
			   game.getFEN() == "1rbqkbnr/Pp2pppp/3P4/8/8/8/P1nQ1PPP/RNB1KBNR w - - 1 9";
	}

	bool legalPromotionsWork(const std::string &fen, const Position &from,
							 const Position &to, bool startsInCheck)
	{
		for (PieceType choice : promotionChoices)
		{
			Game game;
			game.FENToBoard(fen);
			const Color color = game.getCurrentTurn();
			const std::string before = ChessFenUtils::boardToFEN(game.getBoard(), GameState());
			if (ChessAttackUtils::isInCheck(game.getBoard(), color) != startsInCheck)
				return false;
			const std::vector<Position> legal = game.legalMovesFrom(from);
			if (std::find(legal.begin(), legal.end(), to) == legal.end() ||
				ChessFenUtils::boardToFEN(game.getBoard(), GameState()) != before)
				return false;
			game.makeMove(from, to, choice);
			const Piece *promoted = game.getBoard()[to.getX()][to.getY()].getPiece();
			if (promoted == nullptr || promoted->getType() != choice ||
				promoted->getColor() != color ||
				!game.getBoard()[from.getX()][from.getY()].isEmpty() ||
				ChessAttackUtils::isInCheck(game.getBoard(), color) ||
				game.getCurrentTurn() != oppositeColor(color))
				return false;
			game.undoMove();
			if (ChessFenUtils::boardToFEN(game.getBoard(), GameState()) != before ||
				game.getCurrentTurn() != color)
				return false;
		}
		return true;
	}

	bool runTest(const std::string &name, bool result)
	{
		std::cout << (result ? "PASS: " : "FAIL: ") << name << '\n';
		return result;
	}
}

int main()
{
	bool allPassed = true;

	allPassed &= runTest("black rook checks white king",
						 isCheck(Color::White, std::make_unique<Rook>(Color::Black), Position(4, 0)));
	allPassed &= runTest("white rook checks black king",
						 isCheck(Color::Black, std::make_unique<Rook>(Color::White), Position(4, 0)));
	allPassed &= runTest("black bishop checks white king",
						 isCheck(Color::White, std::make_unique<Bishop>(Color::Black), Position(1, 1)));
	allPassed &= runTest("white bishop checks black king",
						 isCheck(Color::Black, std::make_unique<Bishop>(Color::White), Position(1, 1)));
	allPassed &= runTest("black queen checks white king",
						 isCheck(Color::White, std::make_unique<Queen>(Color::Black), Position(4, 0)));
	allPassed &= runTest("white queen checks black king",
						 isCheck(Color::Black, std::make_unique<Queen>(Color::White), Position(4, 0)));
	allPassed &= runTest("black knight checks white king",
						 isCheck(Color::White, std::make_unique<Knight>(Color::Black), Position(2, 3)));
	allPassed &= runTest("white knight checks black king",
						 isCheck(Color::Black, std::make_unique<Knight>(Color::White), Position(2, 3)));
	allPassed &= runTest("black pawn checks white king",
						 isCheck(Color::White, std::make_unique<Pawn>(Color::Black), Position(3, 3)));
	allPassed &= runTest("white pawn checks black king",
						 isCheck(Color::Black, std::make_unique<Pawn>(Color::White), Position(5, 3)));
	allPassed &= runTest("black king checks white king",
						 isCheck(Color::White, std::make_unique<King>(Color::Black), Position(3, 3)));
	allPassed &= runTest("white king checks black king",
						 isCheck(Color::Black, std::make_unique<King>(Color::White), Position(3, 3)));
	allPassed &= runTest("a piece blocks a rook check", blockedRookIsNotCheck());
	allPassed &= runTest("a protected queen checkmates the king",
						 protectedQueenCheckmatesKing());
	allPassed &= runTest("a king escape prevents checkmate",
						 checkWithKingEscapeIsNotCheckmate());
	allPassed &= runTest("stalemate is not checkmate", stalemateIsNotCheckmate());
	allPassed &= runTest("an ordinary position is neither checkmate nor stalemate",
						 ordinaryPositionIsNeitherCheckmateNorStalemate());
	allPassed &= runTest("the fifty-move rule is a draw after 100 half-moves",
						 fiftyMoveRuleUses100HalfMoves());
	allPassed &= runTest("undo restores a captured piece's movement state",
						 undoRestoresCapturedPieceMovementState());
	allPassed &= runTest("a king cannot be captured", kingCannotBeCaptured());
	allPassed &= runTest("pawn double moves require the starting rank",
						 pawnDoubleStepRequiresStartingRank());
	allPassed &= runTest("starting a new game clears move history",
						 aNewGameClearsMoveHistory());
	allPassed &= runTest("loading a FEN clears move history",
						 loadingFenClearsMoveHistory());

	allPassed &= runTest("rejected promotions preserve the pawn, captures, and game state",
						 rejectedPromotionsPreservePosition());
	allPassed &= runTest("legal-move and checkmate queries never promote a checked pawn",
						 promotionQueriesPreservePosition());
	allPassed &= runTest("the screenshot knight check leaves the a7 pawn unchanged",
						 givingCheckDoesNotPromoteOpponentPawn());
	allPassed &= runTest("white can promote by capturing the checking rook",
		legalPromotionsWork("1r2K3/P7/7k/8/8/8/8/8 w - - 0 1", Position("a7"), Position("b8"), true));
	allPassed &= runTest("black can promote by capturing the checking rook",
		legalPromotionsWork("8/8/8/8/8/7K/p7/1R2k3 b - - 0 1", Position("a2"), Position("b1"), true));
	allPassed &= runTest("white can promote by blocking check",
		legalPromotionsWork("K6r/3P4/7k/8/8/8/8/8 w - - 0 1", Position("d7"), Position("d8"), true));
	allPassed &= runTest("black can promote by blocking check",
		legalPromotionsWork("8/8/8/8/8/7K/3p4/k6R b - - 0 1", Position("d2"), Position("d1"), true));
	allPassed &= runTest("ordinary white promotions still work",
		legalPromotionsWork("4k3/P7/8/8/8/8/8/4K3 w - - 0 1", Position("a7"), Position("a8"), false));
	allPassed &= runTest("ordinary black promotions still work",
		legalPromotionsWork("4k3/8/8/8/8/8/p7/4K3 b - - 0 1", Position("a2"), Position("a1"), false));

	return allPassed ? 0 : 1;
}
