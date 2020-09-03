#include "plantime.h"

#include <QString>
#include <QDebug>

PlanTime::PlanTime(): hour(0), minute(0)
{
}

PlanTime::PlanTime(int h, int m)
  : hour(h), minute(m)
{
}

PlanTime PlanTime::operator+(const PlanTime &time)
{
  int sumAsMinutes = time.hour * 60 + time.minute + hour * 60 + time.minute;
  sumAsMinutes = (sumAsMinutes % (24 * 60));

  PlanTime sumPlanTime = PlanTime(sumAsMinutes / 60, sumAsMinutes % 60);
  return sumPlanTime;
}

PlanTime PlanTime::operator-(const PlanTime &time)
{
  int subAsMinutes = (hour * 60 + minute) - (time.hour * 60 + time.minute);
  subAsMinutes = (24 * 60 + subAsMinutes) % (24 * 60);

  PlanTime subPlanTime = PlanTime(subAsMinutes / 60, subAsMinutes % 60);
  return subPlanTime;
}

int PlanTime::asMinutes()
{
  return hour * 60 + minute;
}

QString PlanTime::toString()
{
  QString hourStr = "";
  QString minuteStr = "";

  if(hour < 10){
    hourStr += "0";
  }
  hourStr += QString::number(hour);

  if(minute < 10){
    minuteStr += "0";
  }
  minuteStr += QString::number(minute);

  return hourStr + ":" + minuteStr;
}

PlanTime *PlanTime::parseTime(const QString timeString, const char delimiter)
{
  int i = 0;
  int parsedHour = 0;
  while(timeString[i] != delimiter){
    parsedHour *= 10;
    parsedHour += timeString[i].unicode() - '0';

    i++;
  }

  if(timeString[i] != delimiter){
      return nullptr;
  }

  int parsedMinute = 0;
  int strLen = timeString.length();
  i += 1;
  while(i < strLen){
    parsedMinute *= 10;
    parsedMinute += timeString[i].unicode() - '0';

    i++;
  }

  PlanTime *planTime = new PlanTime(parsedHour, parsedMinute);

  return planTime;
}

QTime PlanTime::toQTime()
{
  return QTime(hour, minute);
}

bool PlanTime::areSame(const PlanTime &another)
{
  if(hour != another.hour){
    return false;
  }
  if(minute != another.minute){
    return false;
  }

  return true;
}

