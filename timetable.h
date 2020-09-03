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


class Timetable : public QWidget
{
  Q_OBJECT
public:
  bool allShown = false;
  int labelWidths[7] = {-1, -1, -1, -1, -1, -1, -1};

  ReservedPlan reservedPlans[24][60];

  QMediaPlayer *bellPlayer = new QMediaPlayer();

  QVBoxLayout *layout;
  DayFrame *dayFrames[7];

  explicit Timetable(QWidget *parent = nullptr);
  void setPlan(Plan *newPlan);
  void playBell(const QUrl bellPath);
  void loadFromJson(const QByteArray json);
  QJsonArray exportAsJson(void);

signals:
  void planClicked(PlanFrame*);
  void planStartedMessage(CharacterWords::Timings);
  void planEndedMessage(CharacterWords::Timings);
  void planPrelimMessage(CharacterWords::Timings);

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
  void processPlanTimings(const QDateTime &currentTime, bool dayChanged);
  void bellProperBell(const QTime currentTime);
  void switchHighlightedDay(int day);
  void deletePlanFrame(PlanFrame *plan);
  void addPlan(Plan *newPlan);
};


#endif // TIMETABLE_H
