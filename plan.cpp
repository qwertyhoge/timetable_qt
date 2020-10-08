#include "plan.h"

#include <QJsonObject>
#include <QJsonArray>

Plan::Plan()
{
}

Plan::Plan(QString name, PlanTime *start, PlanTime *end, DayConsts::DayNums day, QVector<QDir>& dirs)
  : planName(name), startTime(start), endTime(end), dayNum(day), workingDirs(dirs)
{
}

Plan::~Plan()
{
  delete startTime;
  delete endTime;
}

QString Plan::getPlanName(void) const
{
  return planName;
}

PlanTime Plan::getStartTime() const
{
  return *startTime;
}

PlanTime Plan::getEndTime() const
{
  return *endTime;
}

DayConsts::DayNums Plan::getDayNum() const
{
  return dayNum;
}

QString Plan::dirsAsString() const
{
  QString dirsStr = "";
  for(auto dir : workingDirs){
    qDebug("appending " + dir.path().toUtf8());
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

void Plan::setDayNum(DayConsts::DayNums newDayNum)
{
  dayNum = newDayNum;
}

void Plan::addWorkingDir(QString newPath)
{
  QDir newDir(newPath);
  workingDirs.push_back(newDir);
}

bool Plan::removeWorkingDir(QString pathToRemove)
{
  for(int i = 0; i < workingDirs.length(); i++){
    if(pathToRemove == workingDirs[i].path()){
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
