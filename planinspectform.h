#ifndef PLANINSPECTFORM_H
#define PLANINSPECTFORM_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>

class Plan;

class PlanInspectForm : public QWidget
{
  Q_OBJECT

  enum EditStates{
    IDLE,
    INSPECT,
    EDIT
  };

public:
  explicit PlanInspectForm(QWidget *parent = nullptr);
  void inspectPlan(const Plan *plan);

private:
  QFormLayout *layout;

  QLineEdit *nameEdit;
  QLabel *nameLabel;
  QComboBox *dayCombo;
  QLabel *dayLabel;
  QTimeEdit *startTimeEdit;
  QLabel *startTimeLabel;
  QTimeEdit *endTimeEdit;
  QLabel *endTimeLabel;
  QLineEdit *workingDirEdit;
  QLabel *workingDirLabel;

  QPushButton *editButton;
  QPushButton *cancelButton;
  QPushButton *deleteButton;

  EditStates currentState;
  const Plan *inspectingPlan;

  void switchState(EditStates newState);
signals:
  void planDeleteQuery();

public slots:

private slots:
  void startEdit();
  void applyEdit();
  void cancelEdit();
};

#endif // PLANINSPECTFORM_H
