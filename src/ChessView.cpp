#include "ChessView.hpp"

#include <QPainter>
#include <QResizeEvent>
#include <QShowEvent>

ChessView::ChessView(QGraphicsScene *scene, QWidget *parent)
	: QGraphicsView(scene, parent)
{
	setAlignment(Qt::AlignCenter);
	setFrameShape(QFrame::NoFrame);
	setStyleSheet(QStringLiteral("QGraphicsView { background-color: #141916; }"));
	setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
				   QPainter::SmoothPixmapTransform);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ChessView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
	fitScene();
}

void ChessView::showEvent(QShowEvent *event)
{
	QGraphicsView::showEvent(event);
	fitScene();
}

void ChessView::fitScene()
{
	if (scene())
		fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}