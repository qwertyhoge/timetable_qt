#ifndef PLAN_H
#define PLAN_H

#include <QDir>
#include <QVector>

#include "plantime.h"

class Plan
{
public:
  Plan();
  Plan(QString name, PlanTime *start, PlanTime *end, int day, QVector<QDir>& dirs);

  QString getPlanName(void);
  PlanTime getStartTime();
  PlanTime getEndTime();
  int getDayNum();
  QString dirsAsString() const;

  void setPlanName(QString newPlanName);
  void setTimes(PlanTime newStartTime, PlanTime newEndTime);
  void setDayNum(int newDayNum);

  void addWorkingDir(QDir newDir);
  bool removeWorkingDir(QDir dirToRemove);
  QJsonObject getJsonObj();

  bool areSame(const Plan &another);
private:
  QString planName;
  PlanTime *startTime;
  PlanTime *endTime;
  int dayNum;
  QVector<QDir> workingDirs;

signals:

public slots:
};

#endif // PLAN_H
