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

    dayPlans.push_back(planInfo);
  }

  return dayPlans;
}
