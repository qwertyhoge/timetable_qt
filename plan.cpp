#include "plan.h"

#include <QJsonObject>
#include <QJsonArray>

Plan::Plan()
{
}

Plan::Plan(QString name, PlanTime *start, PlanTime *end, int day, QVector<QDir>& dirs)
  : planName(name), startTime(start), endTime(end), dayNum(day), workingDirs(dirs)
{

}


QString Plan::getPlanName(void)
{
  return planName;
}

PlanTime Plan::getStartTime()
{
  return *startTime;
}

PlanTime Plan::getEndTime()
{
  return *endTime;
}

int Plan::getDayNum()
{
  return dayNum;
}

QString Plan::dirsAsString() const
{
  QString dirsStr = "";
  for(auto dir : workingDirs){
    dirsStr += ";";
    dirsStr += dir.path();
  }
  dirsStr.remove(0, 1);

  return dirsStr;
}

void Plan::setPlanName(QString newPlanName)
{
  planName = newPlanName;
}

void Plan::setTimes(PlanTime newStartTime, PlanTime newEndTime)
{
  startTime = &newStartTime;
  endTime = &newEndTime;
}

void Plan::setDayNum(int newDayNum)
{
  dayNum = newDayNum;
}

void Plan::addWorkingDir(QDir newDir)
{
  workingDirs.push_back(newDir);
}

bool Plan::removeWorkingDir(QDir dirToRemove)
{
  for(int i = 0; i < workingDirs.length(); i++){
    if(dirToRemove.path() == workingDirs[i].path()){
      workingDirs.removeAt(i);
      return true;
    }
  }

  return false;
}

QJsonObject Plan::getJsonObj()
{
  QJsonObject obj;
  obj.insert("planName", planName);
  obj.insert("startTime", startTime->toString());
  obj.insert("endTime", endTime->toString());
  QJsonArray dirArray;
  for(QDir &dir : workingDirs){
    dirArray.push_back(dir.path());
  }

  obj.insert("workingDirectories", dirArray);

  return obj;
}

bool Plan::areSame(const Plan &another)
{
  if(planName != another.planName){
    return false;
  }

  if(!startTime->areSame(*another.startTime)){
    return false;
  }
  if(!endTime->areSame(*another.endTime)){
    return false;
  }

  if(dayNum != another.dayNum){
    return false;
  }

  if(dirsAsString() != another.dirsAsString()){
    return false;
  }

  return true;
}
