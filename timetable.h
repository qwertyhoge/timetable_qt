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

#include "planframe.h"
#include "dayframe.h"
#include "reservedplan.h"

#include "characterwords.h"

enum Days{
  SUNDAY = 0,
  MONDAY = 1,
  TUESDAY = 2,
  WEDNESDAY = 3,
  THURSDAY = 4,
  FRIDAY = 5,
  SATURDAY = 6
};

class Timetable : public QWidget
{
  Q_OBJECT
public:
  QMap<QString, int> dayMap;
  QString dayStrings[7] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
  };
  bool allShown = false;
  int labelWidths[7] = {-1, -1, -1, -1, -1, -1, -1};

  ReservedPlan reservedPlans[24][60];

  QMediaPlayer *bellPlayer = new QMediaPlayer();

  QVBoxLayout *layout;
  DayFrame *dayFrames[7];

  explicit Timetable(QWidget *parent = nullptr);
  void setPlan(Plan *newPlan);
  void playBell(QUrl bellPath);
  void loadFromJson(QByteArray json);
  QJsonArray exportAsJson(void);

signals:
  void planClicked(PlanFrame*);
  void planStartedMessage(CharacterWords::Timings);
  void planEndedMessage(CharacterWords::Timings);
  void planPrelimMessage(CharacterWords::Timings);

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
  void processPlanTimings(QDateTime &currentTime, bool dayChanged);
  void bellProperBell(QTime currentTime);
  void switchHighlightedDay(int day);
  void deletePlan(PlanFrame *plan);
  void addPlan(Plan *newPlan);

private slots:
  void raisePlanClicked(PlanFrame* clickedPlan);
};


#endif // TIMETABLE_H
