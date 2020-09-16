#ifndef TIMENOTIFIER_H
#define TIMENOTIFIER_H

#include <QWidget>
#include <QLabel>
#include <QDateTime>

#include "dayconsts.h"

class TimeNotifier : public QLabel
{
  Q_OBJECT
private:
  DayConsts::DayNums formerDay;
  int formerMinute;
  QDateTime currentDateTime;

public:
  explicit TimeNotifier(QWidget *parent = nullptr);
  DayConsts::DayNums getCurrentDay();

signals:
  void minuteChanged(const QTime &time, DayConsts::DayNums dayNum, bool dayChanged);
public slots:
  void updateTime();
};

#endif // TIMENOTIFIER_H
