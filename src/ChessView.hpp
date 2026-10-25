#pragma once

#include <QGraphicsView>
#include <QPainter>
#include <QResizeEvent>
#include <QShowEvent>

// A QGraphicsView that always keeps its whole scene visible.
class ChessView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit ChessView(QGraphicsScene *scene, QWidget *parent = nullptr)
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

protected:
	void resizeEvent(QResizeEvent *event) override
	{
		QGraphicsView::resizeEvent(event);
		fitScene();
	}

	void showEvent(QShowEvent *event) override
	{
		QGraphicsView::showEvent(event);
		fitScene();
	}

private:
	void fitScene()
	{
		if (scene())
			fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
	}
};
