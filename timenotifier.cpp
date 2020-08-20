#include "timenotifier.h"

#include <QLabel>
#include <QTimer>
#include <QDebug>

TimeNotifier::TimeNotifier(QWidget *parent) : QLabel(parent)
{
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
  timer->start();

  QDateTime initialDateTime = QDateTime::currentDateTime();

  formerDay = initialDateTime.date().dayOfWeek() % 7;
  formerMinute = initialDateTime.time().minute();

  setText(initialDateTime.time().toString());
}

void TimeNotifier::updateTime()
{
  QDateTime currentDateTime = QDateTime::currentDateTime();
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
