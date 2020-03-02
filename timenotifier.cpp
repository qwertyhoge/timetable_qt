#include "timenotifier.h"

#include <QLabel>
#include <QTimer>

TimeNotifier::TimeNotifier(QWidget *parent) : QLabel(parent)
{
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
  timer->start();

  QDateTime initialDateTime = QDateTime::currentDateTime();

  formerDay = initialDateTime.date().day() - 1;
  formerMinute = initialDateTime.time().minute();

  setText(initialDateTime.time().toString());
}

void TimeNotifier::updateTime()
{
  QDateTime currentDateTime = QDateTime::currentDateTime();
  int currentDay = currentDateTime.date().day() - 1;
  int currentMinute = currentDateTime.time().minute();

  setText(currentDateTime.time().toString());

  if(formerDay != currentDay){
    emit dayChanged(currentDay);
  }
  if(formerMinute != currentMinute){
    emit minuteChanged(currentDateTime.time());
  }

  formerDay = currentDay;
  formerMinute = currentMinute;
}
