#ifndef TIME_H
#define TIME_H

#include <QString>

class PlanTime
{
public:
  PlanTime();
  PlanTime(int h, int m = 0);

  int hour;
  int minute;

  int operator+(PlanTime time);
  int operator-(PlanTime time);

  QString toString();
};

#endif // TIME_H
