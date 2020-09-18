#include "planinspectform.h"
#include "dayconsts.h"

PlanInspectForm::PlanInspectForm(QWidget *parent)
  : QWidget(parent), currentState(IDLE), inspectingPlan(nullptr)
{
  layout = new QFormLayout();

  setLayout(layout);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

  nameEdit = new QLineEdit();
  nameLabel = new QLabel(tr("Plan Name"));
  startTimeEdit = new QTimeEdit();
  startTimeLabel = new QLabel(tr("Start Time"));
  endTimeEdit = new QTimeEdit();
  endTimeLabel = new QLabel(tr("End Time"));
  workingDirEdit = new QLineEdit();
  workingDirLabel = new QLabel(tr("Workiing Directory"));
  dayCombo = new QComboBox();
  dayLabel = new QLabel(tr("Day"));

  for(int i = 0; i < 7; i++){
    dayCombo->addItem(DayConsts::dayStrings[i]);
  }
  dayCombo->setCurrentIndex(-1);

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
  buttonContainer->setLayout(containerLayout);
  containerLayout->addWidget(editButton);
  containerLayout->addWidget(cancelButton);

  layout->addRow(deleteButton, buttonContainer);
}
