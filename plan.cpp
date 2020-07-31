#include "plan.h"

#include "plantime.h"

#include <QLayout>
#include <QLabel>
#include <QFrame>
#include <QDebug>

Plan::Plan(QString name, PlanTime *start, PlanTime *end, int day, QVector<QDir> &dir)
  : QFrame(nullptr), planName(name), startTime(start), endTime(end), dayNum(day), workingDirs(dir)
{
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
}

void Plan::updateText()
{
  nameLabel->setText(planName);
  startTimeLabel->setText(startTime->toString());
  endTimeLabel->setText(endTime->toString());

  fitGeometry();
}

QRect Plan::fitGeometry()
{
  QRect geometry;
  QSize parentSize = parentWidget()->size();
  int startMinutes = startTime->asMinutes();
  int endMinutes = endTime->asMinutes();

  if(endMinutes == 0){
    endMinutes = 24 * 60;
  }

  xPos = startMinutes * parentSize.width() / 24 / 60;
  yPos = 0;

  geometry.setX(xPos);
  geometry.setY(yPos);

  geometry.setWidth((endMinutes - startMinutes) * parentSize.width() / 24 / 60);
  geometry.setHeight(parentSize.height());

  setGeometry(geometry);

  return geometry;
}

void Plan::updatePlanGeometry()
{
  fitGeometry();
}

void Plan::mousePressEvent(QMouseEvent *event){
  emit planClicked(this);
}
