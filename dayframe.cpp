#include "dayframe.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

DayFrame::DayFrame(QWidget *parent, QString dayString)
  : QFrame(parent)
{
  setFrameStyle(QFrame::Box);
  setLineWidth(1);

  this->dayString = dayString;

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
  for(auto plan : plans){
    plan->updatePlanGeometry();
  }
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
      reservedPlans[i][j].bellType = NONE_BELL;
      reservedPlans[i][j].planRef = nullptr;
    }
  }

  for(auto plan : plans){
    PlanTime *start = plan->startTime;
    PlanTime *end = plan->endTime;
    PlanTime *prelim = start - 5;

    if(reservedPlans[prelim->hour][prelim->minute].bellType == NONE_BELL){
      reservedPlans[prelim->hour][prelim->minute].bellType = PRELIM_BELL;
      reservedPlans[prelim->hour][prelim->minute].planRef = plan;
    }
    if(reservedPlans[end->hour][end->minute].bellType != START_BELL){
      reservedPlans[end->hour][end->minute].bellType = END_BELL;
      reservedPlans[end->hour][end->minute].planRef = plan;
    }
    reservedPlans[start->hour][start->minute].bellType = START_BELL;
    reservedPlans[start->hour][start->minute].planRef = plan;
  }
}

void DayFrame::addPlan(Plan *newPlan)
{
  plans.push_back(newPlan);
  newPlan->setParent(planArea);
  newPlan->fitGeometry();

  newPlan->show();
}

void DayFrame::deletePlan(Plan *plan)
{
  if(!plans.removeOne(plan)){
    qCritical() << "failed to delete from vector";
  }
  plan->setParent(nullptr);
  plan->deleteLater();
}

void DayFrame::clearPlans(void)
{
  for(auto plan : plans){
    deletePlan(plan);
  }
}

QJsonArray DayFrame::extractDayJsonArray()
{
  QJsonArray dayPlans;
  for(auto plan : plans){
    QJsonObject planInfo;
    qDebug() << "capsling " << plan->planName;

    planInfo.insert("planName", plan->planName);
    planInfo.insert("startTime", plan->startTime->toString());
    planInfo.insert("endTime", plan->endTime->toString());
    QJsonArray dirArray;
    for(QDir &dir : plan->workingDirs){
      dirArray.push_back(dir.path());
    }
    planInfo.insert("workingDirectories", dirArray);

    dayPlans.push_back(planInfo);
  }

  return dayPlans;
}

int DayFrame::labelWidth()
{
  return dayLabel->width();
}

void DayFrame::setLabelWidth(int labelWidth)
{
  dayLabel->setFixedWidth(labelWidth);
}
