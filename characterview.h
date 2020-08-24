#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsView>
#include <QImage>
#include <QRect>
#include <QHoverEvent>

class CharacterView : public QGraphicsView
{
  Q_OBJECT

public:
  CharacterView(QWidget *parent = nullptr);

private:
  QRect imageRect;
  QPoint characterPoint;
  QRect boundingRect;
  QImage usualPicture;
  QImage hoveredPicture;
  QImage pressedPicture;

  bool characterHovered = false;
  bool characterPressing = false;

protected:
  void paintEvent(QPaintEvent*) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

  bool event(QEvent *event) override;

signals:
  void characterClicked();
};

#endif // CHARACTER_H
