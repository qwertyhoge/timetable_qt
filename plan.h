#ifndef PLAN_H
#define PLAN_H

#include <QWidget>
#include <plantime.h>

class Plan : public QWidget
{
  Q_OBJECT
public:
  explicit Plan(QWidget *parent = nullptr);
  Plan(QWidget *parent, QString name, PlanTime start, PlanTime end);

signals:

public slots:

private:
  int xPos = 0;
  int yPos;

  QString planName;

  PlanTime startTime;
  PlanTime endTime;
};

#endif // PLAN_H
