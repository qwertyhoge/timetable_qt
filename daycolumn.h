#ifndef DAYCOLUMN_H
#define DAYCOLUMN_H

#include <QFrame>
#include <QLayout>
#include <QLabel>

#include "plan.h"

class DayColumn : public QFrame
{
  Q_OBJECT

public:
  DayColumn(QWidget *parent = nullptr);

  int day;
  QString dayString;

  void addPlan(Plan* plan);

private:
  QHBoxLayout *layout;
  QLabel *dayLabel;
  QWidget *planArea;
};

#endif // DAYCOLUMN_H
