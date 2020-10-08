#ifndef PLAN_H
#define PLAN_H

#include <QDir>
#include <QVector>

#include "plantime.h"
#include "dayconsts.h"

class Plan
{
public:
  Plan();
  Plan(QString name, PlanTime *start, PlanTime *end, DayConsts::DayNums day, QVector<QDir>& dirs);
  ~Plan();

  QString getPlanName(void) const;
  PlanTime getStartTime() const;
  PlanTime getEndTime() const;
  DayConsts::DayNums getDayNum() const;
  QString dirsAsString() const;

  void setPlanName(QString newPlanName);
  void setTimes(PlanTime newStartTime, PlanTime newEndTime);
  void setDayNum(DayConsts::DayNums newDayNum);

  void addWorkingDir(QString newPath);
  bool removeWorkingDir(QString pathToRemove);
  QJsonObject getJsonObj();

  bool areSame(const Plan &another);
private:
  QString planName;
  PlanTime *startTime;
  PlanTime *endTime;
  DayConsts::DayNums dayNum;
  QVector<QDir> workingDirs;

signals:

public slots:
};

#endif // PLAN_H
