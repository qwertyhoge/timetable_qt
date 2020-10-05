#include "planinspectform.h"
#include "dayconsts.h"
#include <QDebug>

#include "plan.h"

PlanInspectForm::PlanInspectForm(QWidget *parent)
  : QWidget(parent), currentState(IDLE), inspectingPlan(nullptr)
{
  layout = new QFormLayout();
  layout->setVerticalSpacing(8);
  layout->setContentsMargins(2, 0, 2, 0);

  setLayout(layout);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

  nameEdit = new QLineEdit();
  nameLabel = new QLabel(tr("Plan Name"));
  startTimeEdit = new QTimeEdit();
  startTimeLabel = new QLabel(tr("Start Time"));
  endTimeEdit = new QTimeEdit();
  endTimeLabel = new QLabel(tr("End Time"));
  workingDirEdit = new QLineEdit();
  workingDirLabel = new QLabel(tr("Working Directory"));
  workingDirEdit->setReadOnly(true);
  dayCombo = new QComboBox();
  dayLabel = new QLabel(tr("Day"));

  for(int i = 0; i < 7; i++){
    dayCombo->addItem(DayConsts::dayStrings[i]);
  }

  layout->addRow(nameLabel, nameEdit);
  layout->addRow(dayLabel, dayCombo);
  layout->addRow(startTimeLabel, startTimeEdit);
  layout->addRow(endTimeLabel, endTimeEdit);
  layout->addRow(workingDirLabel, workingDirEdit);

  editButton = new QPushButton(tr("Edit Plan"));
  cancelButton = new QPushButton(tr("Cancel"));
  deleteButton = new QPushButton(tr("Delete Plan"));
  QWidget *buttonContainer = new QWidget();
  QHBoxLayout *containerLayout = new QHBoxLayout();
  containerLayout->setContentsMargins(0, 0, 0, 0);
  containerLayout->setSpacing(6);
  buttonContainer->setLayout(containerLayout);
  containerLayout->addWidget(editButton);
  containerLayout->addWidget(cancelButton);

  layout->addRow(deleteButton, buttonContainer);

  connect(deleteButton, SIGNAL(clicked()), this, SLOT(deletePlan()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelEdit()));

  switchState(IDLE);
}

void PlanInspectForm::deletePlan()
{
  inspectingPlan = nullptr;
  switchState(IDLE);

  emit currentPlanDeleteQuery();
}

void PlanInspectForm::inspectPlan(const Plan *plan)
{
  inspectingPlan = plan;
  switchState(INSPECT);

}

void PlanInspectForm::switchState(EditStates newState)
{
  nameEdit->setEnabled(false);
  startTimeEdit->setEnabled(false);
  endTimeEdit->setEnabled(false);
  workingDirEdit->setEnabled(false);
  dayCombo->setEnabled(false);
  deleteButton->setEnabled(false);
  editButton->setEnabled(false);
  cancelButton->setEnabled(false);

  editButton->setText("Edit Plan");

  disconnect(editButton, SIGNAL(clicked()), this, nullptr);

  switch(newState){
    case IDLE:
      nameEdit->setText("");
      startTimeEdit->setTime(QTime(0, 0));
      endTimeEdit->setTime(QTime(0, 0));
      workingDirEdit->setText("");
      dayCombo->setCurrentIndex(-1);
      break;
    case INSPECT:
      nameEdit->setText(inspectingPlan->getPlanName());
      startTimeEdit->setTime(inspectingPlan->getStartTime().toQTime());
      endTimeEdit->setTime(inspectingPlan->getEndTime().toQTime());
      workingDirEdit->setText(inspectingPlan->dirsAsString());
      dayCombo->setCurrentIndex(inspectingPlan->getDayNum());

      editButton->setEnabled(true);
      deleteButton->setEnabled(true);

      connect(editButton, SIGNAL(clicked()), this, SLOT(startEdit()));
      break;
    case EDIT:
      nameEdit->setEnabled(true);
      startTimeEdit->setEnabled(true);
      endTimeEdit->setEnabled(true);
      workingDirEdit->setEnabled(false); // editting this is not implemented yet
      dayCombo->setEnabled(true);
      deleteButton->setEnabled(true);
      editButton->setEnabled(true);
      cancelButton->setEnabled(true);

      editButton->setText("Apply Edit");
      connect(editButton, SIGNAL(clicked()), this, SLOT(applyEdit()));
      break;
  }
}

void PlanInspectForm::startEdit()
{
  switchState(EDIT);
}

void PlanInspectForm::applyEdit()
{
  QString planName = nameEdit->text();
  PlanTime *startTime = new PlanTime(startTimeEdit->time());
  PlanTime *endTime = new PlanTime(endTimeEdit->time());
  DayConsts::DayNums dayNum = DayConsts::intToDayNums(dayCombo->currentIndex());
  QVector<QDir> dirVec;
  if(!workingDirEdit->text().isEmpty()){
    for(auto path : workingDirEdit->text().split(';')){
      dirVec.push_back(QDir(path));
    }
  }

  Plan *newPlan = new Plan(planName, startTime, endTime, dayNum, dirVec);
  qDebug() << "editted dayNum: " << dayNum;
  inspectPlan(newPlan);

  emit currentPlanChangeQuery(newPlan);
}

void PlanInspectForm::cancelEdit()
{
  switchState(INSPECT);
}
