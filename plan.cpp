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
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  QPalette plt = palette();
  plt.setColor(QPalette::Background, QColor(225, 255, 252));
  setAutoFillBackground(true);
  setPalette(plt);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->setSpacing(4);
  layout->setContentsMargins(0, 6, 0, 6);
  setLayout(layout);

  QLabel *nameLabel = new QLabel(planName);
  nameLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  nameLabel->setAlignment(Qt::AlignCenter);
  QFont font;
  font.setPointSize(12);
  nameLabel->setFont(font);
  layout->addWidget(nameLabel);

  QFrame *horLine = new QFrame();
  horLine->setFrameShape(QFrame::HLine);
  horLine->setFrameShadow(QFrame::Sunken);
  horLine->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  horLine->setFixedHeight(2);
  horLine->setLineWidth(2);
  layout->addWidget(horLine);

  layout->addStretch(1);

  QLabel *timeLabel = new QLabel(start.toString() + " - " + end.toString());
  layout->addWidget(timeLabel);
  timeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  timeLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);
}
