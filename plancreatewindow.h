#ifndef PLANCREATEWINDOW_H
#define PLANCREATEWINDOW_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QTimeEdit>
#include <QComboBox>

#include "plan.h"

class PlanCreateWindow : public QDialog
{
  Q_OBJECT

public:
  PlanCreateWindow(QWidget *parent = nullptr);

private:
  QFormLayout *formLayout;
  QLineEdit *planName;
  QTimeEdit *startTime;
  QTimeEdit *endTime;
  QComboBox *daySelect;

private slots:
  void sendPlan();

signals:
  void planMake(Plan*);
};

#endif // PLANCREATEWINDOW_H
