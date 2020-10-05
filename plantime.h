#ifndef PLANTIME_H
#define PLANTIME_H

#include <QString>
#include <QTime>

class PlanTime
{
public:
  PlanTime ();
  PlanTime (int h, int m = 0);
  PlanTime (const QTime &qTime);

  int hour;
  int minute;

  PlanTime operator+(const PlanTime &time);
  PlanTime operator-(const PlanTime &time);

  int asMinutes();
  QString toString();

  bool areSame(const PlanTime &another);
  QTime toQTime();

  static PlanTime *parseTime(const QString timeString, const char delimiter);
};

#endif // PLANTIME_H
