#include "plan.h"

#include "plantime.h"

#include <QLayout>
#include <QLabel>

Plan::Plan(QWidget *parent) : QWidget(parent), planName(""), startTime(0), endTime(0)
{
}

Plan::Plan(QWidget *parent, QString name, PlanTime start, PlanTime end)
  : QWidget(parent), planName(name), startTime(start), endTime(end)
{
  setStyleSheet("background-color: #f7eff2;");

  QVBoxLayout *layout = new QVBoxLayout();
  layout->setSpacing(6);
  setLayout(layout);

  QLabel *nameLabel = new QLabel(planName);
  layout->addWidget(nameLabel);

  /*
  QFrame *horLine = new QFrame();
  horLine->setFrameShape(QFrame::HLine);
  horLine->setFrameShadow(QFrame::Sunken);
  horLine->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  horLine->setFixedHeight(2);
  horLine->setLineWidth(2);
  layout->addWidget(horLine);
*/
  QLabel *timeLabel = new QLabel(start.toString() + " - " + end.toString());
  layout->addWidget(timeLabel);

  layout->addStretch(1);

  QLabel *extraLabel = new QLabel("test");
  layout->addWidget(extraLabel);
}
