#include "timenotifier.h"

#include <QLabel>
#include <QTimer>
#include <QDebug>

TimeNotifier::TimeNotifier(QWidget *parent) : QLabel(parent), currentDateTime(QDateTime::currentDateTime())
{
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
  timer->start();

  formerDay = DayConsts::intToDayNums(currentDateTime.date().dayOfWeek() % 7);
  formerMinute = currentDateTime.time().minute();

  setText(currentDateTime.time().toString());
}

DayConsts::DayNums TimeNotifier::getCurrentDay(){
  return formerDay;
}

void TimeNotifier::updateTime()
{
  currentDateTime = QDateTime::currentDateTime();
  DayConsts::DayNums currentDay = DayConsts::intToDayNums(currentDateTime.date().dayOfWeek() % 7);
  int currentMinute = currentDateTime.time().minute();

  setText(currentDateTime.time().toString());

  bool dayChanged = false;
  if(formerDay != currentDay){
    dayChanged = true;
  }
  if(formerMinute != currentMinute){
    emit minuteChanged(currentDateTime.time(), currentDay, dayChanged);
  }

  formerDay = currentDay;
  formerMinute = currentMinute;
}
