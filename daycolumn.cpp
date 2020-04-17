#include "daycolumn.h"

DayColumn::DayColumn(QWidget *parent)
  : QFrame(parent)
{
  setFrameStyle(QFrame::Box);
  setLineWidth(1);

  layout = new QHBoxLayout();
  layout->setMargin(0);
  setLayout(layout);

  dayLabel = new QLabel("DAY");
  dayLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  dayLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  dayLabel->setLineWidth(2);
  dayLabel->setMargin(0);
  layout->addWidget(dayLabel);

  planArea = new QWidget();
  layout->addWidget(planArea);
}

void DayColumn::addPlan(Plan *plan)
{

}
