#ifndef PLAN_H
#define PLAN_H

#include <QWidget>
#include <plantime.h>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QMap>

class Plan : public QFrame
{
  Q_OBJECT
public:
  QString planName;
  PlanTime *startTime;
  PlanTime *endTime;
  int dayNum;

  Plan(QString name, PlanTime *start, PlanTime *end, int day);

  void updatePlanGeometry();
  void updateText();
  QRect fitGeometry();
signals:
  void planClicked(Plan *plan);

private:
  int xPos;
  int yPos = 0;

  QLabel *nameLabel;
  QLabel *startTimeLabel;
  QLabel *endTimeLabel;

protected:
  void mousePressEvent(QMouseEvent *event) override;
};

#endif // PLAN_H
