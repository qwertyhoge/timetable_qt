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
#include "registeredplan.h"

#include "characterwords.h"


class Timetable : public QWidget
{
  Q_OBJECT
public:
  explicit Timetable(QWidget *parent = nullptr);
  void setPlan(Plan *newPlan);
  void playBell(const QUrl bellPath);
  void loadFromJson(const QByteArray json);
  QJsonArray exportAsJson(void);

private:
  bool allShown = false;
  int maxDayFrameLabelWidth = 0;
  int dayFrameLabelWidths[7] = {-1, -1, -1, -1, -1, -1, -1};

  RegisteredPlan registeredPlans[24][60];

  QVBoxLayout *layout;
  DayFrame *dayFrames[7];

  QMediaPlayer *bellPlayer = new QMediaPlayer();

signals:
  void planClicked(PlanFrame*);
  void planStartedMessage(CharacterWords::Timings);
  void planEndedMessage(CharacterWords::Timings);
  void planPrelimMessage(CharacterWords::Timings);

public slots:
  void justifyDayFrameLabel(const int labelWidth, DayConsts::DayNums dayNum);

  void processPlanTimings(const QDateTime &currentTime, bool dayChanged);
  void bellProperBell(const QTime currentTime);
  void switchHighlightedDay(DayConsts::DayNums day);
  void deletePlanFrame(PlanFrame *plan);
  void addPlan(Plan *newPlan);
};


#endif // TIMETABLE_H
