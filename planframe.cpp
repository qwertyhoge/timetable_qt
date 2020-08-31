#include "planframe.h"

#include "plantime.h"

#include <QLayout>
#include <QLabel>
#include <QFrame>
#include <QDebug>

PlanFrame::PlanFrame()
  : QFrame(nullptr)
{
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

  nameLabel = new QLabel();
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

  startTimeLabel = new QLabel();
  layout->addWidget(startTimeLabel);
  startTimeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  startTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  layout->addStretch(1);

  QHBoxLayout *bottomLayout = new QHBoxLayout();
  layout->addLayout(bottomLayout);

  endTimeLabel = new QLabel();
  bottomLayout->addWidget(endTimeLabel);
  endTimeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  endTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
}

void PlanFrame::attachPlan(Plan *plan)
{
  planData = plan;
  nameLabel->setText(plan->getPlanName());
  startTimeLabel->setText(plan->getStartTime().toString());
  endTimeLabel->setText(plan->getEndTime().toString());

  fitGeometry();
}

QRect PlanFrame::fitGeometry()
{
  QRect geometry;
  QSize parentSize = parentWidget()->size();
  int startMinutes = planData->getStartTime().asMinutes();
  int endMinutes = planData->getEndTime().asMinutes();

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

Plan *PlanFrame::getPlanData()
{
  return planData;
}

void PlanFrame::updatePlanGeometry()
{
  fitGeometry();
}

void PlanFrame::mousePressEvent(QMouseEvent *event){
  emit planClicked(this);
}
