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
  QString dirsAsString();

  void setName(QString planName);
  void setTime(PlanTime &start, PlanTime &end);

  void addWorkingDir(QDir newDir);
  bool removeWorkingDir(QDir dirToRemove);
  QJsonObject getJsonObj();

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
