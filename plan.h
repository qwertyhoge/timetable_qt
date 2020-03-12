#ifndef PLAN_H
#define PLAN_H

#include <QWidget>
#include <plantime.h>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

class Plan : public QFrame
{
  Q_OBJECT
public:
  int day;

  QString planName;

  PlanTime *startTime;
  PlanTime *endTime;

  explicit Plan(QWidget *parent = nullptr);
  Plan(QWidget *parent, QString name, PlanTime *start, PlanTime *end);

  void updatePlanGeometry();
signals:
  void deleteButtonClicked(Plan *plan);

public slots:
  void emitDeleteSignal();

private:
  int xPos;
  int yPos = 0;

  QLabel *nameLabel;
  QLabel *startTimeLabel;
  QLabel *endTimeLabel;
  QPushButton *deleteButton;

  QRect calculateGeometry(QRect parentGeometry);
};

#endif // PLAN_H
