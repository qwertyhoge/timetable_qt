#include "timenotifier.h"

#include <QLabel>
#include <QTimer>
#include <QDebug>

TimeNotifier::TimeNotifier(QWidget *parent) : QLabel(parent), currentDateTime(QDateTime::currentDateTime())
{
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
  timer->start();

  formerDay = currentDateTime.date().dayOfWeek() % 7;
  formerMinute = currentDateTime.time().minute();

  setText(currentDateTime.time().toString());
}

int TimeNotifier::getCurrentDay(){
  return currentDateTime.date().dayOfWeek() % 7;
}

void TimeNotifier::updateTime()
{
  currentDateTime = QDateTime::currentDateTime();
  int currentDay = currentDateTime.date().dayOfWeek() % 7;
  int currentMinute = currentDateTime.time().minute();

  setText(currentDateTime.time().toString());

  bool dayChanged = false;
  if(formerDay != currentDay){
    dayChanged = true;
  }
  if(formerMinute != currentMinute){
    emit minuteChanged(currentDateTime, dayChanged);
  }

  formerDay = currentDay;
  formerMinute = currentMinute;
}
