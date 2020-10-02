#ifndef DAYFRAME_H
#define DAYFRAME_H

#include <QFrame>
#include <QLayout>
#include <QLabel>

#include "planframe.h"
#include "registeredplan.h"
#include "dayconsts.h"

class DayFrame : public QFrame
{
  Q_OBJECT

public:
  DayFrame(DayConsts::DayNums day);

  void highlight();
  void lowlight();
  void addPlan(Plan *planFrame);
  void assignExistingPlanFrame(PlanFrame *planFrame);
  bool deletePlanFrame(PlanFrame *planFrame);
  bool releasePlanFrame(PlanFrame *planFrame);
  void clearPlans();
  QJsonArray extractDayJsonArray();
  void fillReservedPlans(RegisteredPlan registeredPlans[24][60]);

  int labelWidth();
  void setLabelWidth(const int labelWidth);
  void updateAllPlanFramesGeometry(void);

  int planAreaWidth();

private:
  QHBoxLayout *layout;

  QLabel *dayLabel;
  QWidget *planArea;

  QVector<PlanFrame*> planFrames;
  QString dayString;

  DayConsts::DayNums dayNum;

signals:
  void labelWidthDefined(const int labelWidth, DayConsts::DayNums dayNum);

  void planClicked(PlanFrame*);
protected:
  bool eventFilter(QObject *obj, QEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void showEvent(QShowEvent *event) override;
};

#endif // DAYFRAME_H
