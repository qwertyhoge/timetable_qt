#ifndef DAYFRAME_H
#define DAYFRAME_H

#include <QFrame>
#include <QLayout>
#include <QLabel>

#include "planframe.h"
#include "reservedplan.h"

class DayFrame : public QFrame
{
  Q_OBJECT

public:
  DayFrame(QWidget *parent, int day, QString dayString);

  void highlight();
  void lowlight();
  void addPlan(Plan *plan);
  void deletePlanFrame(PlanFrame *plan);
  void clearPlans();
  QJsonArray extractDayJsonArray();
  void fillReservedPlans(ReservedPlan reservedPlans[24][60]);

  int labelWidth();
  void setLabelWidth(const int labelWidth);

private:
  QHBoxLayout *layout;

  QLabel *dayLabel;
  QWidget *planArea;

  QVector<PlanFrame*> planFrames;
  QString dayString;

  int dayNum;

signals:
  void labelWidthDefined(const int labelWidth, int dayNum);

  void planClicked(PlanFrame*);
protected:
  void resizeEvent(QResizeEvent *event) override;
  void showEvent(QShowEvent *event) override;
};

#endif // DAYFRAME_H
