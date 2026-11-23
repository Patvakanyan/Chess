#pragma once

#include <QChar>
#include <QGraphicsScene>
#include <QHash>
#include <QPainterPath>
#include <QPointF>
#include <QVector>

#include <array>
#include <functional>
#include <optional>

class QGraphicsPathItem;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;

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

	explicit ChessScene(QObject *parent = nullptr, bool blackAtBottom = false);
	~ChessScene() override;

	// Board coordinates always match the engine: row 0 is rank 8 and column 0
	// is the a-file. Display orientation does not change these coordinates.
	void showPosition(const Board &board);

	// Places (or removes, with std::nullopt) a single piece.
	void setPiece(int row, int column, std::optional<PieceView> piece);
	std::optional<PieceView> pieceAt(int row, int column) const;

	// Slides the existing graphics item to its destination, then calls finished.
	// Captures, en passant, and the rook part of castling are handled here too.
	void animateMove(int fromRow, int fromColumn,
					 int toRow, int toColumn,
					 std::function<void()> finished);

	// Highlights a set of squares (e.g. legal move targets). x = column, y = row.
	void highlightSquares(const QVector<QPoint> &squares,
						  const QVector<QPoint> &castleSquares = {});
	void clearHighlights();
	static QPixmap pieceIcon(PieceKind kind, bool white, int size);
	// Center of a square in the displayed grid, with row 0 at the top.
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
	QPointF displayedSquareCenter(int row, int column) const;
	void clearPieces();
	void removePieceItem(int row, int column);
	QGraphicsPathItem *createPieceItem(const PieceView &piece, int row, int column);
	const QPainterPath &glyphPath(PieceKind kind);

	bool m_blackAtBottom;
	Board m_board{};
	ItemGrid m_items{};
	QHash<int, QPainterPath> m_glyphCache;
	QVector<QGraphicsItem *> m_highlightItems;
};
