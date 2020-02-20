#ifndef PLAN_H
#define PLAN_H

#include <QWidget>
#include <plantime.h>
#include <QFrame>

#include <QLabel>

class Plan : public QFrame
{
  Q_OBJECT
public:
  explicit Plan(QWidget *parent = nullptr);
  Plan(QWidget *parent, QString name, PlanTime start, PlanTime end);

  void updatePlanGeometry();
signals:

public slots:

private:
  int xPos = 0;
  int yPos;

  QString planName;

  PlanTime startTime;
  PlanTime endTime;

  QLabel *nameLabel;
  QLabel *timeLabel;

  QRect calculateGeometry(QRect parentGeometry);
};

#endif // PLAN_H
