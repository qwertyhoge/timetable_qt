#ifndef TIMENOTIFIER_H
#define TIMENOTIFIER_H

#include <QWidget>
#include <QLabel>
#include <QDateTime>

class TimeNotifier : public QLabel
{
  Q_OBJECT
private:

public:
  QDateTime currentDateTime;
  int formerDay;
  int formerMinute;

  explicit TimeNotifier(QWidget *parent = nullptr);

signals:
  void minuteChanged(QDateTime &dateTime, bool dayChanged);
public slots:
  void updateTime();
};

#endif // TIMENOTIFIER_H
