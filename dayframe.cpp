#include "dayframe.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

DayFrame::DayFrame(DayConsts::DayNums day)
  : QFrame(nullptr)
{
  setFrameStyle(QFrame::Box);
  setLineWidth(1);

  dayNum = day;
  this->dayString = DayConsts::dayStrings[day];

  layout = new QHBoxLayout();
  layout->setMargin(0);
  layout->setSpacing(0);
  setLayout(layout);

  dayLabel = new QLabel(dayString);
  dayLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
  dayLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  dayLabel->setAlignment(Qt::AlignCenter);
  dayLabel->setLineWidth(2);
  dayLabel->setMargin(0);
  dayLabel->setAutoFillBackground(true);
  layout->addWidget(dayLabel);

  planArea = new QWidget();
  layout->addWidget(planArea);
}

void DayFrame::resizeEvent(QResizeEvent *event)
{
  for(auto plan : planFrames){
    plan->updatePlanGeometry();
  }
}

void DayFrame::showEvent(QShowEvent *event)
{
  emit labelWidthDefined(labelWidth(), dayNum);
}

void DayFrame::highlight()
{
  QPalette plt = palette();

  plt.setColor(QPalette::Background, QColor(253, 217, 192));

  dayLabel->setPalette(plt);
}

void DayFrame::lowlight()
{
  QPalette plt = palette();

  plt.setColor(QPalette::Background, QColor(248, 251, 254));

  dayLabel->setPalette(plt);
}

void DayFrame::fillReservedPlans(ReservedPlan reservedPlans[24][60])
{
  for(int i = 0; i < 24; i++){
    for(int j = 0; j < 60; j++){
      reservedPlans[i][j].clear();
    }
  }

  for(auto pf : planFrames){
    Plan *plan = pf->getPlanData();
    PlanTime start = plan->getStartTime();
    PlanTime end = plan->getEndTime();
    PlanTime *prelim = &start - 5;

    reservedPlans[prelim->hour][prelim->minute].addReserve(PRELIM_BELL, plan);
    reservedPlans[end.hour][end.minute].addReserve(END_BELL, plan);
    reservedPlans[start.hour][start.minute].addReserve(START_BELL, plan);
  }
}

void DayFrame::addPlan(Plan *newPlan)
{
  PlanFrame *newPlanFrame = new PlanFrame();

  planFrames.push_back(newPlanFrame);
  newPlanFrame->setParent(planArea);

  newPlanFrame->attachPlan(newPlan);

  connect(newPlanFrame, SIGNAL(planClicked(PlanFrame*)), this, SIGNAL(planClicked(PlanFrame*)));

  newPlanFrame->show();
}

void DayFrame::deletePlanFrame(PlanFrame *plan)
{
  PlanFrame *toBeRemoved = nullptr;
  for(int i = 0; i < planFrames.length(); i++){
    if(planFrames[i] == plan){
      toBeRemoved = planFrames[i];
    }
  }

  if(toBeRemoved != nullptr){
    planFrames.removeOne(toBeRemoved);

    toBeRemoved->setParent(nullptr);
    toBeRemoved->deleteLater();
  }else{
    qCritical() << "failed to remove from plan frame vector.";
  }
}

void DayFrame::clearPlans(void)
{
  for(auto plan : planFrames){
    deletePlanFrame(plan);
  }
}

QJsonArray DayFrame::extractDayJsonArray()
{
  QJsonArray dayPlans;
  for(auto plan : planFrames){
    QJsonObject planInfo = plan->getPlanData()->getJsonObj();

    dayPlans.push_back(planInfo);
  }

  return dayPlans;
}

int DayFrame::labelWidth()
{
  return dayLabel->width();
}

void DayFrame::setLabelWidth(const int labelWidth)
{
  dayLabel->setFixedWidth(labelWidth);
}
