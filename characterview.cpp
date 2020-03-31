#include "characterview.h"

#include <QApplication>
#include <QDebug>

CharacterView::CharacterView(QWidget *parent)
  : QGraphicsView(parent)
{
  usualPicture.load(":/character/proto/stand_usual.png");
  usualPicture = usualPicture.convertToFormat(QImage::Format_ARGB32);
  hoveredPicture.load(":/character/proto/stand_delight.png");
  hoveredPicture = hoveredPicture.convertToFormat(QImage::Format_ARGB32);
  pressedPicture.load(":/character/proto/stand_smile.png");
  pressedPicture = pressedPicture.convertToFormat(QImage::Format_ARGB32);

  imageRect = QRect(0, 0, usualPicture.width(), usualPicture.height());

  setAttribute(Qt::WA_Hover);
}

void CharacterView::paintEvent(QPaintEvent *event)
{
  QPainter painter(viewport());

  int paintAreaLength = qMin(width() - 5, height() - 5);
  float aspect = float(usualPicture.width()) / usualPicture.height();
  int characterHeight = paintAreaLength;
  int characterWidth = int(paintAreaLength * aspect);

  characterPoint = QPoint((width() - characterWidth) / 2, (height() - characterHeight) / 2);
  boundingRect = QRect(characterPoint.x(),characterPoint.y(), characterWidth, characterHeight);

  if(characterPressing){
    painter.drawImage(boundingRect, pressedPicture, imageRect);
  }else{
    if(characterHovered){
      painter.drawImage(boundingRect, hoveredPicture, imageRect);
    }else{
      painter.drawImage(boundingRect, usualPicture, imageRect);
    }
  }

}

void CharacterView::mousePressEvent(QMouseEvent *event)
{
  if(boundingRect.contains(event->pos())){
    characterPressing = true;
    viewport()->repaint();
  }
}

void CharacterView::mouseMoveEvent(QMouseEvent *event)
{
  if(!boundingRect.contains(event->pos()) && characterPressing){
    characterPressing = false;
    viewport()->repaint();
  }
}

void CharacterView::mouseReleaseEvent(QMouseEvent *event)
{
  if(characterPressing){
    qDebug() << "open menu";
  }

  if(characterPressing){
    characterPressing = false;
    viewport()->repaint();
  }
}

bool CharacterView::event(QEvent *event)
{
  if(event->type() == QEvent::HoverMove){
    QHoverEvent *hoverEvent = static_cast<QHoverEvent *>(event);
    if(boundingRect.contains(hoverEvent->pos()) && !characterHovered){
      characterHovered = true;
      viewport()->repaint();
    }else if(!boundingRect.contains(hoverEvent->pos()) && characterHovered){
      characterHovered = false;
      viewport()->repaint();
    }

    return true;
  }

  if(event->type() == QEvent::HoverLeave){
    if(characterHovered){
      characterHovered = false;
      viewport()->repaint();

      return true;
    }
  }

  return QGraphicsView::event(event);
}
