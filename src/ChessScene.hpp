#pragma once

#include <QChar>
#include <QGraphicsScene>
#include <QHash>
#include <QPainterPath>
#include <QPointF>
#include <QVector>

#include <array>
#include <optional>

class QGraphicsPathItem;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;

// Named PieceKind/PieceView (not PieceType/Piece) because the chess engine
// this scene talks to already defines its own PieceType and Piece classes.
enum class PieceKind
{
	King,
	Queen,
	Rook,
	Bishop,
	Knight,
	Pawn
};

struct PieceView
{
	PieceKind kind;
	bool white;
};

class ChessScene : public QGraphicsScene
{
	Q_OBJECT

public:
	static constexpr int kSquareSize = 80;
	static constexpr int kBoardSize = 8;
	static constexpr int kBoardPixels = kSquareSize * kBoardSize;

	using Board = std::array<std::array<std::optional<PieceView>, kBoardSize>, kBoardSize>;

	explicit ChessScene(QObject *parent = nullptr);

	// Redraws every piece to match the given position. row 0 is rank 8,
	// column 0 is the a-file -- the same convention the chess engine uses,
	// so board rows/columns can be passed straight through with no flipping.
	void showPosition(const Board &board);

	// Places (or removes, with std::nullopt) a single piece.
	void setPiece(int row, int column, std::optional<PieceView> piece);
	std::optional<PieceView> pieceAt(int row, int column) const;

	// Highlights a set of squares (e.g. legal move targets). x = column, y = row.
	void highlightSquares(const QVector<QPoint> &squares,
						  const QVector<QPoint> &castleSquares = {});
	void clearHighlights();

	// The single place that maps a board square to scene coordinates.
	static QPointF squareCenter(int row, int column);

signals:
	// Emitted on a left click inside the board; row/column follow the same
	// convention as showPosition/setPiece.
	void squareClicked(int row, int column);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
	enum ZLayer
	{
		ZSquare = 0,
		ZHighlight = 2,
		ZPiece = 3,
		ZLabel = 4
	};

	using ItemGrid = std::array<std::array<QGraphicsPathItem *, kBoardSize>, kBoardSize>;

	void drawBoardSquares();
	void drawCoordinates();
	void clearPieces();
	QGraphicsPathItem *createPieceItem(const PieceView &piece, int row, int column);
	const QPainterPath &glyphPath(PieceKind kind);

	Board m_board{};
	ItemGrid m_items{};
	QHash<int, QPainterPath> m_glyphCache;
	QVector<QGraphicsItem *> m_highlightItems;
};
