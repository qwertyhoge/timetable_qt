#ifndef TIMENOTIFIER_H
#define TIMENOTIFIER_H

#include <QWidget>
#include <QLabel>
#include <QDateTime>

class TimeNotifier : public QLabel
{
  Q_OBJECT
private:
  int formerDay;
  int formerMinute;
  QDateTime currentDateTime;

public:
  explicit TimeNotifier(QWidget *parent = nullptr);
  int getCurrentDay();

signals:
  void minuteChanged(const QDateTime &dateTime, bool dayChanged);
public slots:
  void updateTime();
};

#endif // TIMENOTIFIER_H
