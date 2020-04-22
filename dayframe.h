#ifndef DAYFRAME_H
#define DAYFRAME_H

#include <QFrame>
#include <QLayout>
#include <QLabel>

#include "plan.h"

class DayFrame : public QFrame
{
  Q_OBJECT

public:
  DayFrame(QWidget *parent = nullptr, QString dayString = "DAY");

  QString dayString;
  QVector<Plan*> plans;

  QLabel *dayLabel;
  QWidget *planArea;

  void addPlan(Plan* plan);
  void deletePlan(Plan* plan);
  void updateText();
  void clearPlans(void);
  QJsonArray extractDayJsonArray(void);

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  QHBoxLayout *layout;
};

#endif // DAYFRAME_H
