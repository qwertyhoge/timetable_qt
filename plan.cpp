#include "plan.h"

#include "plantime.h"

#include <QLayout>
#include <QLabel>
#include <QFrame>
#include <QDebug>

Plan::Plan(QWidget *parent) : QFrame(parent), planName("")
{
  startTime = new PlanTime(0);
  endTime = new PlanTime(0);
}

Plan::Plan(QWidget *parent, QString name, PlanTime *start, PlanTime *end)
  : QFrame(parent), planName(name), startTime(start), endTime(end)
{
  qDebug() << "parent: " << parent;
  qDebug() << "name: " << name;
  qDebug() << "start: " << start->hour << ":" << start->minute;
  qDebug() << "end: " << end->hour << ":" << end->minute;

  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  setFrameStyle(QFrame::Plain | QFrame::Box);
  setLineWidth(1);

  QPalette plt = palette();
  plt.setColor(QPalette::Background, QColor(225, 255, 252));
  setAutoFillBackground(true);
  setPalette(plt);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->setSpacing(4);
  layout->setContentsMargins(0, 6, 0, 6);
  setLayout(layout);

  nameLabel = new QLabel(planName);
  nameLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  nameLabel->setAlignment(Qt::AlignCenter);
  QFont font;
  font.setPointSize(13);
  nameLabel->setFont(font);
  layout->addWidget(nameLabel);

  QFrame *horLine = new QFrame();
  horLine->setFrameShape(QFrame::HLine);
  horLine->setFrameShadow(QFrame::Sunken);
  horLine->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
  horLine->setFixedHeight(2);
  horLine->setLineWidth(2);
  layout->addWidget(horLine);

  startTimeLabel = new QLabel(start->toString());
  layout->addWidget(startTimeLabel);
  startTimeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
  startTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  layout->addStretch(1);

  QHBoxLayout *bottomLayout = new QHBoxLayout();
  layout->addLayout(bottomLayout);

  endTimeLabel = new QLabel(end->toString());
  bottomLayout->addWidget(endTimeLabel);
  endTimeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
  endTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

  bottomLayout->addStretch(1);

  deleteButton = new QPushButton(tr("Delete"));
  bottomLayout->addWidget(deleteButton);
  deleteButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  connect(deleteButton, SIGNAL(clicked()), this, SLOT(emitDeleteSignal()));

  QRect geometry = calculateGeometry(parent->geometry());
  setGeometry(geometry);

  show();
}

QRect Plan::calculateGeometry(QRect parentGeometry)
{
  QRect geometry;
  int startMinutes = startTime->asMinutes();
  int endMinutes = endTime->asMinutes();

  if(endMinutes == 0){
    endMinutes = 24 * 60;
  }

  geometry.setX(0);
  geometry.setY(startMinutes * parentGeometry.height() / 24 / 60);

  geometry.setWidth(parentGeometry.width());
  geometry.setHeight((endMinutes - startMinutes) * parentGeometry.height() / 24 / 60);

  return geometry;
}

void Plan::updatePlanGeometry()
{
  QRect newGeometry = calculateGeometry(parentWidget()->geometry());

  setGeometry(newGeometry);
}

void Plan::emitDeleteSignal()
{
  emit deleteButtonClicked(this);
}
