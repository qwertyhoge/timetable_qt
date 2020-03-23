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
  nameLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  nameLabel->setAlignment(Qt::AlignCenter);
  QFont font;
  font.setPointSize(13);
  nameLabel->setFont(font);
  nameLabel->raise();
  layout->addWidget(nameLabel);

  QFrame *horLine = new QFrame();
  horLine->setFrameShape(QFrame::HLine);
  horLine->setFrameShadow(QFrame::Sunken);
  horLine->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
  horLine->setLineWidth(2);
  layout->addWidget(horLine);

  startTimeLabel = new QLabel(start->toString());
  layout->addWidget(startTimeLabel);
  startTimeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  startTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  layout->addStretch(1);

  QHBoxLayout *bottomLayout = new QHBoxLayout();
  layout->addLayout(bottomLayout);

  endTimeLabel = new QLabel(end->toString());
  bottomLayout->addWidget(endTimeLabel);
  endTimeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  endTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

  QRect geometry = calculateGeometry(parent->geometry());
  setGeometry(geometry);

  show();
}

void Plan::updateData()
{
  nameLabel->setText(planName);
  startTimeLabel->setText(startTime->toString());
  endTimeLabel->setText(endTime->toString());

  updatePlanGeometry();
}

QRect Plan::calculateGeometry(QRect parentGeometry)
{
  QRect geometry;
  int startMinutes = startTime->asMinutes();
  int endMinutes = endTime->asMinutes();

  if(endMinutes == 0){
    endMinutes = 24 * 60;
  }

  xPos = startMinutes * parentGeometry.width() / 24 / 60;
  yPos = 0;

  geometry.setX(xPos);
  geometry.setY(yPos);

  geometry.setWidth((endMinutes - startMinutes) * parentGeometry.width() / 24 / 60);
  geometry.setHeight(parentGeometry.height());

  return geometry;
}

void Plan::updatePlanGeometry()
{
  QRect newGeometry = calculateGeometry(parentWidget()->geometry());

  setGeometry(newGeometry);
}

void Plan::mousePressEvent(QMouseEvent *event){
  emit planClicked(this);
}
