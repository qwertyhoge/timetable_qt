#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QVector>
#include <QMap>

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QLayout>

#include <QTime>
#include <QUrl>
#include <QMediaPlayer>
#include <QEvent>

#include "plan.h"
#include "dayframe.h"

class Timetable : public QWidget
{
  Q_OBJECT
public:
  enum Days{
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THIRSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6
  };
  enum BellType{
    NONE_BELL,
    START_BELL,
    END_BELL,
    PRELIM_BELL
  };
  QMap<QString, int> dayMap;
  QString dayStrings[7] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thirsday",
    "Friday",
    "Saturday"
  };

  BellType timeToAlerm[24][60];
  QVector<Plan*> timetableData[7];

  QMediaPlayer *bellPlayer = new QMediaPlayer();

  QVBoxLayout *layout;
  DayFrame *dayFrames[7];

  explicit Timetable(QWidget *parent = nullptr);
  void setPlan(Plan *newPlan);
  void playBell(QUrl bellPath);
  void loadFromJson(QByteArray json);
  QJsonArray exportAsJson(void);

signals:
  void planClicked(Plan*);

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
  void bellProperBell(QTime currentTime);
  void highlightCurrentDay(int day);
  void deletePlan(Plan *plan);
  void addPlan(Plan *newPlan);

private slots:
  void propagatePlanClicked(Plan* clickedPlan);
};

#endif // TIMETABLE_H
