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

  BellType timeToAlerm[24][60];

  QMap<QString, int> dayMap;
  QFrame *dayFrames[7];
  QLabel *dayLabels[7];
  QVector<Plan*> timetableData[7];
  QMediaPlayer *bellPlayer = new QMediaPlayer();

  QVBoxLayout *layout;

  explicit Timetable(QWidget *parent = nullptr);
  void setPlan(Plan *newPlan, int dayNum);
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

private slots:
  void propagatePlanClicked(Plan* clickedPlan);
};

#endif // TIMETABLE_H
