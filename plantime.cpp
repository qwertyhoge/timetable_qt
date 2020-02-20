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

int PlanTime::operator+(PlanTime time)
{
  return time.hour * 60 + time.minute + hour * 60 + time.minute;
}

int PlanTime::operator-(PlanTime time)
{
  return (hour * 60 + minute) - (time.hour * 60 + time.minute);
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
