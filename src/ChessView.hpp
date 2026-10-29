#pragma once

#include <QGraphicsView>

class QGraphicsScene;
class QResizeEvent;
class QShowEvent;

// A QGraphicsView that always keeps its whole scene visible.
class ChessView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit ChessView(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
	void resizeEvent(QResizeEvent *event) override;
	void showEvent(QShowEvent *event) override;

private:
	void fitScene();
};
