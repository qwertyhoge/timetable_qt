#ifndef PLANCREATEWINDOW_H
#define PLANCREATEWINDOW_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QTimeEdit>
#include <QComboBox>

#include "planframe.h"

#include "characterwords.h"

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
  QLineEdit *workingDir;
  QVector<QDir> workingDirectories;

private slots:
  void sendPlan();
  void openWorkingDirDialog();

signals:
  void planCreated(Plan*);
  void planCreatedMessage(CharacterWords::Timings planMakeTim);
};

#endif // PLANCREATEWINDOW_H
