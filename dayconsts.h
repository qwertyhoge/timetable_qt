#ifndef DAYCONSTS_H
#define DAYCONSTS_H

#include "QString"
#include "QStringList"


namespace DayConsts {
  static const QStringList dayStrings = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
  };
  enum DayNums{
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6
  };

};

#endif // DAYCONSTS_H
