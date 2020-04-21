#include "dayframe.h"

DayFrame::DayFrame(QWidget *parent, QString dayString)
  : QFrame(parent)
{
  setFrameStyle(QFrame::Box);
  setLineWidth(1);

  this->dayString = dayString;

  layout = new QHBoxLayout();
  layout->setMargin(0);
  setLayout(layout);

  dayLabel = new QLabel(dayString);
  dayLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  dayLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  dayLabel->setLineWidth(2);
  dayLabel->setMargin(0);
  layout->addWidget(dayLabel);

  planArea = new QWidget();
  layout->addWidget(planArea);
}

void DayFrame::addPlan(Plan *plan)
{

}
