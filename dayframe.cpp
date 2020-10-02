#include "dayframe.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QEvent>

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
  dayLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  dayLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  dayLabel->setAlignment(Qt::AlignCenter);
  dayLabel->setLineWidth(2);
  dayLabel->setMargin(0);
  dayLabel->setAutoFillBackground(true);
  layout->addWidget(dayLabel);

  planArea = new QWidget();
  layout->addWidget(planArea);

  planArea->installEventFilter(this);
}

void DayFrame::resizeEvent(QResizeEvent *event)
{
  updateAllPlanFramesGeometry();
}

void DayFrame::showEvent(QShowEvent *event)
{
  emit labelWidthDefined(labelWidth(), dayNum);
}

bool DayFrame::eventFilter(QObject *obj, QEvent *event)
{
  if(obj == planArea && event->type() == QEvent::Resize){
    updateAllPlanFramesGeometry();
  }
  return false;
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

void DayFrame::fillReservedPlans(RegisteredPlan registeredPlans[24][60])
{
  for(int i = 0; i < 24; i++){
    for(int j = 0; j < 60; j++){
      registeredPlans[i][j].clear();
    }
  }

  for(auto pf : planFrames){
    Plan *plan = pf->getPlanData();
    PlanTime start = plan->getStartTime();
    PlanTime end = plan->getEndTime();
    PlanTime *prelim = &start - 5;

    registeredPlans[prelim->hour][prelim->minute].registerPlan(PRELIM_BELL, plan);
    registeredPlans[end.hour][end.minute].registerPlan(END_BELL, plan);
    registeredPlans[start.hour][start.minute].registerPlan(START_BELL, plan);
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

void DayFrame::assignExistingPlanFrame(PlanFrame *planFrame)
{
  planFrames.push_back(planFrame);
  planFrame->setParent(planArea);

  connect(planFrame, SIGNAL(planClicked(PlanFrame*)), this, SIGNAL(planClicked(PlanFrame*)));

  planFrame->show();
}

bool DayFrame::deletePlanFrame(PlanFrame *planFrame)
{
  int removedIndex = -1;
  PlanFrame *toBeRemoved = nullptr;
  for(int i = 0; i < planFrames.length(); i++){
    if(planFrames[i] == planFrame){
      toBeRemoved = planFrames[i];
      removedIndex = i;
    }
  }

  if(toBeRemoved != nullptr){
    planFrames.removeAt(removedIndex);
    disconnect(planFrame, SIGNAL(planClicked(PlanFrame*)), this, SIGNAL(planClicked(PlanFrame*)));

    toBeRemoved->setParent(nullptr);
    toBeRemoved->deleteLater();

    return true;
  }else{
    qCritical() << "failed to remove from plan frame vector.";
  }

  return false;
}

bool DayFrame::releasePlanFrame(PlanFrame *planFrame)
{
  PlanFrame *toBeRemoved = nullptr;
  int removedIndex = -1;
  for(int i = 0; i < planFrames.length(); i++){
    if(planFrames[i] == planFrame){
      toBeRemoved = planFrames[i];
      removedIndex = i;
    }
  }

  if(toBeRemoved != nullptr){
    planFrames.removeAt(removedIndex);
    disconnect(planFrame, SIGNAL(planClicked(PlanFrame*)), this, SIGNAL(planClicked(PlanFrame*)));

    return true;
  }

  return false;
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
  dayLabel->setMinimumWidth(labelWidth);
  layout->update();
}

void DayFrame::updateAllPlanFramesGeometry(void)
{
  for(auto pf : planFrames){
    pf->updatePlanGeometry();
  }
}


int DayFrame::planAreaWidth()
{
  return planArea->width();
}
