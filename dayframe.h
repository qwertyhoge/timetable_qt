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
  DayFrame(QWidget *parent = nullptr, QString dayString = "DAY");

  void highlight();
  void lowlight();
  void addPlan(Plan *plan);
  void deletePlanFrame(PlanFrame *plan);
  void updateText();
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

signals:
  void planClicked(PlanFrame*);

protected:
  void resizeEvent(QResizeEvent *event) override;
};

#endif // DAYFRAME_H
