#include "plancreatewindow.h"

#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

PlanCreateWindow::PlanCreateWindow(QWidget *parent)
  : QDialog(parent)
{
  formLayout = new QFormLayout();
  setLayout(formLayout);

  planName = new QLineEdit();
  QLabel *planLabel = new QLabel("Plan Name");
  formLayout->addRow(planLabel, planName);

  daySelect = new QComboBox();
  daySelect->addItem("");
  daySelect->addItem("Sunday");
  daySelect->addItem("Monday");
  daySelect->addItem("Tuesday");
  daySelect->addItem("Wednesday");
  daySelect->addItem("Thirsday");
  daySelect->addItem("Friday");
  daySelect->addItem("Saturday");
  QLabel *dayLabel = new QLabel("Day");
  formLayout->addRow(dayLabel, daySelect);

  startTime = new QTimeEdit();
  QLabel *startLabel = new QLabel("Start Time");
  formLayout->addRow(startLabel, startTime);

  endTime = new QTimeEdit();
  QLabel *endLabel = new QLabel("End Time");
  formLayout->addRow(endLabel, endTime);

  QPushButton *makePlanButton = new QPushButton("Make plan");
  connect(makePlanButton, SIGNAL(clicked()), this, SLOT(sendPlan()));
  formLayout->addRow(nullptr, makePlanButton);
}

void PlanCreateWindow::sendPlan()
{
  QString name = planName->text();
  QString startTimeText = startTime->text();
  QString endTimeText = endTime->text();
  QString day = daySelect->currentText();

  if(day == ""){
    // error
    return;
  }

  PlanTime *startTime = PlanTime::parseTime(startTimeText, ':');
  PlanTime *endTime  = PlanTime::parseTime(endTimeText, ':');

  Plan *newPlan = new Plan(name, startTime, endTime, daySelect->currentIndex() - 1);
  planMake(newPlan);
}
