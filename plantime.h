#ifndef TIME_H
#define TIME_H

#include <QString>

class PlanTime
{
public:
  PlanTime ();
  PlanTime (int h, int m = 0);

  int hour;
  int minute;

  PlanTime *operator+(PlanTime *time);
  PlanTime *operator-(PlanTime *time);

  int asMinutes();
  QString toString();

  static PlanTime *parseTime(QString timeString, char delimiter);
};

#endif // TIME_H
