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

  QLabel *dayLabel;
  QWidget *planArea;

  QString dayString;

  void addPlan(Plan* plan);
  void updateText();
private:
  QHBoxLayout *layout;
};

#endif // DAYFRAME_H
