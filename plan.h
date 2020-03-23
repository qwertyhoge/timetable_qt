#ifndef PLAN_H
#define PLAN_H

#include <QWidget>
#include <plantime.h>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

class Plan : public QFrame
{
  Q_OBJECT
public:
  int day;

  QString planName;

  PlanTime *startTime;
  PlanTime *endTime;

  explicit Plan(QWidget *parent = nullptr);
  Plan(QWidget *parent, QString name, PlanTime *start, PlanTime *end);

  void updatePlanGeometry();
  void updateData();
signals:
  void planClicked(Plan *plan);

private:
  int xPos;
  int yPos = 0;

  QLabel *nameLabel;
  QLabel *startTimeLabel;
  QLabel *endTimeLabel;

  QRect calculateGeometry(QRect parentGeometry);

protected:
  void mousePressEvent(QMouseEvent *event) override;
};

#endif // PLAN_H
