#ifndef PLANTIME_H
#define PLANTIME_H

#include <QString>
#include <QTime>

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

  bool areSame(const PlanTime &another);
  QTime toQTime();

  static PlanTime *parseTime(QString timeString, char delimiter);
};

#endif // PLANTIME_H
