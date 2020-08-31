#ifndef PLANFRAME_H
#define PLANFRAME_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QDir>

#include "plan.h"
#include "plantime.h"

class PlanFrame : public QFrame
{
  Q_OBJECT
public:
  PlanFrame();

  void updatePlanGeometry();
  QRect fitGeometry();

  Plan *getPlanData();

  void attachPlan(Plan *plan);
signals:
  void planClicked(PlanFrame *self);

private:
  int xPos;
  int yPos = 0;

  QLabel *nameLabel;
  QLabel *startTimeLabel;
  QLabel *endTimeLabel;

  Plan *planData;

protected:
  void mousePressEvent(QMouseEvent *event) override;
};

#endif // PLANFRAME_H
