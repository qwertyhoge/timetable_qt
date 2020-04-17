#include "timetable.h"

#include <QLabel>

Timetable::Timetable(QWidget *parent)
  : QWidget(parent)
{
  move(0, 0);
  layout = new QVBoxLayout();
  setLayout(layout);
  layout->setSpacing(0);
}
