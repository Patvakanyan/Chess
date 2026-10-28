#include "ChessScene.hpp"

#include <QColor>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QGuiApplication>
#include <QPen>

namespace
{
constexpr qreal kPieceHeightRatio = 0.68;
constexpr qreal kPieceWidthRatio = 0.68;
constexpr int kGlyphReferenceSize = 100; // only used to build the outline; it is rescaled later

const QColor kLightSquare(241, 224, 194);
const QColor kDarkSquare(109, 78, 63);
const QColor kSceneBackground(22, 29, 26);
const QColor kLightSquareLabel(84, 59, 40);
const QColor kDarkSquareLabel(239, 218, 181);
const QColor kWhitePieceFill(0xf3, 0xe5, 0xc8);
const QColor kWhitePieceOutline(0x35, 0x28, 0x1d);
const QColor kBlackPieceFill(0x24, 0x24, 0x24);
const QColor kBlackPieceOutline(0xc8, 0xb9, 0x9c);
const QColor kHighlightColor(120, 200, 140, 140);
const QColor kCastleHighlightColor(245, 205, 55, 190);

constexpr char kFiles[] = "abcdefgh";

// Unicode "black" chess glyphs (solid shapes) are used for both sides;
// fill and outline colors distinguish the teams.
constexpr char16_t glyphFor(PieceKind kind)
{
	switch (kind)
	{
	case PieceKind::King:   return 0x265A;
	case PieceKind::Queen:  return 0x265B;
	case PieceKind::Rook:   return 0x265C;
	case PieceKind::Bishop: return 0x265D;
	case PieceKind::Knight: return 0x265E;
	case PieceKind::Pawn:   return 0x265F;
	}
	return 0;
}

bool isLightSquare(int row, int column)
{
	return (row + column) % 2 == 0;
}

QColor squareColor(int row, int column)
{
	return isLightSquare(row, column) ? kLightSquare : kDarkSquare;
}

QColor coordinateColor(int row, int column)
{
	return isLightSquare(row, column) ? kLightSquareLabel : kDarkSquareLabel;
}

QFont pieceFont()
{
	QFont font;
	font.setFamilies({QStringLiteral("Noto Sans Symbols 2"),
					  QStringLiteral("Segoe UI Symbol"),
					  QStringLiteral("Apple Symbols"),
					  QStringLiteral("DejaVu Sans")});
	font.setPixelSize(kGlyphReferenceSize);
	return font;
}
} // namespace

ChessScene::ChessScene(QObject *parent)
	: QGraphicsScene(parent)
{
	setSceneRect(0, 0, kBoardPixels, kBoardPixels);
	setBackgroundBrush(kSceneBackground);
	drawBoardSquares();
	drawCoordinates();
}

QPointF ChessScene::squareCenter(int row, int column)
{
	return QPointF(column * kSquareSize + kSquareSize / 2.0,
				   row * kSquareSize + kSquareSize / 2.0);
}

void ChessScene::drawBoardSquares()
{
	for (int row = 0; row < kBoardSize; ++row)
	{
		for (int column = 0; column < kBoardSize; ++column)
		{
			const QColor color = squareColor(row, column);
			auto *square = addRect(column * kSquareSize, row * kSquareSize,
								   kSquareSize, kSquareSize);
			square->setBrush(color);
			square->setPen(QPen(color.darker(115), 1));
			square->setZValue(ZSquare);
		}
	}
}

void ChessScene::drawCoordinates()
{
	QFont font = QGuiApplication::font();
	font.setPointSize(9);
	font.setBold(true);

	for (int index = 0; index < kBoardSize; ++index)
	{
		// File letters: bottom-right corner of the bottom row.
		auto *file = addSimpleText(QString(QLatin1Char(kFiles[index])));
		file->setFont(font);
		file->setPos(index * kSquareSize + kSquareSize - 14, kBoardPixels - 20);
		file->setBrush(coordinateColor(kBoardSize - 1, index));
		file->setZValue(ZLabel);

		// Rank numbers: top-left corner of the left column.
		auto *rank = addSimpleText(QString::number(kBoardSize - index));
		rank->setFont(font);
		rank->setPos(4, index * kSquareSize + 2);
		rank->setBrush(coordinateColor(index, 0));
		rank->setZValue(ZLabel);
	}
}

const QPainterPath &ChessScene::glyphPath(PieceKind kind)
{
	const int key = static_cast<int>(kind);
	auto it = m_glyphCache.find(key);
	if (it == m_glyphCache.end())
	{
		QPainterPath path;
		path.addText(0, 0, pieceFont(), QString(QChar(glyphFor(kind))));
		it = m_glyphCache.insert(key, path);
	}
	return it.value();
}

QGraphicsPathItem *ChessScene::createPieceItem(const PieceView &piece, int row, int column)
{
	const QPainterPath &path = glyphPath(piece.kind);
	const QRectF bounds = path.boundingRect(); // tight glyph bounds, independent of font metrics
	if (bounds.isEmpty())
		return nullptr;

	auto *item = addPath(path);
	item->setBrush(piece.white ? kWhitePieceFill : kBlackPieceFill);
	item->setPen(QPen(piece.white ? kWhitePieceOutline : kBlackPieceOutline, 1.8));
	item->setZValue(ZPiece);

	const qreal scale = qMin(kSquareSize * kPieceHeightRatio / bounds.height(),
							 kSquareSize * kPieceWidthRatio / bounds.width());
	item->setTransformOriginPoint(bounds.center());
	item->setScale(scale);
	item->setPos(squareCenter(row, column) - bounds.center());
	return item;
}

void ChessScene::clearPieces()
{
	for (int row = 0; row < kBoardSize; ++row)
	{
		for (int column = 0; column < kBoardSize; ++column)
		{
			if (QGraphicsPathItem *item = m_items[row][column])
			{
				removeItem(item);
				delete item;
				m_items[row][column] = nullptr;
			}
			m_board[row][column].reset();
		}
	}
}

void ChessScene::setPiece(int row, int column, std::optional<PieceView> piece)
{
	if (row < 0 || row >= kBoardSize || column < 0 || column >= kBoardSize)
		return;

	if (QGraphicsPathItem *old = m_items[row][column])
	{
		removeItem(old);
		delete old;
		m_items[row][column] = nullptr;
	}

	m_board[row][column] = piece;
	if (piece)
		m_items[row][column] = createPieceItem(*piece, row, column);
}

std::optional<PieceView> ChessScene::pieceAt(int row, int column) const
{
	if (row < 0 || row >= kBoardSize || column < 0 || column >= kBoardSize)
		return std::nullopt;
	return m_board[row][column];
}

void ChessScene::showPosition(const Board &board)
{
	for (int row = 0; row < kBoardSize; ++row)
		for (int column = 0; column < kBoardSize; ++column)
			setPiece(row, column, board[row][column]);
}

void ChessScene::highlightSquares(const QVector<QPoint> &squares,
								  const QVector<QPoint> &castleSquares)
{
	clearHighlights();
	auto addHighlight = [this](const QPoint &square, const QColor &color)
	{
		const int column = square.x();
		const int row = square.y();
		if (row < 0 || row >= kBoardSize || column < 0 || column >= kBoardSize)
			return;

		const qreal margin = kSquareSize * 0.14;
		auto *dot = addEllipse(column * kSquareSize + margin, row * kSquareSize + margin,
							   kSquareSize - 2 * margin, kSquareSize - 2 * margin);
		dot->setBrush(color);
		dot->setPen(Qt::NoPen);
		dot->setZValue(ZHighlight);
		m_highlightItems.push_back(dot);
	};

	for (const QPoint &square : squares)
		addHighlight(square, kHighlightColor);
	for (const QPoint &square : castleSquares)
		addHighlight(square, kCastleHighlightColor);
}

void ChessScene::clearHighlights()
{
	for (QGraphicsItem *item : std::as_const(m_highlightItems))
	{
		removeItem(item);
		delete item;
	}
	m_highlightItems.clear();
}

void ChessScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		const QPointF p = event->scenePos();
		const int column = static_cast<int>(p.x()) / kSquareSize;
		const int row = static_cast<int>(p.y()) / kSquareSize;
		if (row >= 0 && row < kBoardSize && column >= 0 && column < kBoardSize)
			emit squareClicked(row, column);
	}
	QGraphicsScene::mousePressEvent(event);
}
