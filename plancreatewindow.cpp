#include "plancreatewindow.h"
#include "characterpanel.h"

#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

PlanCreateWindow::PlanCreateWindow(QWidget *parent)
  : QDialog(parent), workingDirectories()
{
  unsigned int flags = windowFlags();
  flags = flags & (~Qt::WindowContextHelpButtonHint);
  setWindowFlags(Qt::WindowFlags(flags));

  formLayout = new QFormLayout();
  setLayout(formLayout);

  planName = new QLineEdit();
  QLabel *planLabel = new QLabel("Plan Name");
  formLayout->addRow(planLabel, planName);

  daySelect = new QComboBox();
  daySelect->addItem("");
  daySelect->addItem(tr("Sunday"));
  daySelect->addItem(tr("Monday"));
  daySelect->addItem(tr("Tuesday"));
  daySelect->addItem(tr("Wednesday"));
  daySelect->addItem(tr("Thirsday"));
  daySelect->addItem(tr("Friday"));
  daySelect->addItem(tr("Saturday"));
  QLabel *dayLabel = new QLabel(tr("Day"));
  formLayout->addRow(dayLabel, daySelect);

  startTime = new QTimeEdit();
  QLabel *startLabel = new QLabel(tr("Start Time"));
  formLayout->addRow(startLabel, startTime);

  endTime = new QTimeEdit();
  QLabel *endLabel = new QLabel(tr("End Time"));
  formLayout->addRow(endLabel, endTime);

  QLabel *dirLabel = new QLabel(tr("Working Directory"));
  workingDir = new QLineEdit();
  workingDir->setReadOnly(true);
  QPushButton *browseButton = new QPushButton(tr("Browse"));
  QHBoxLayout *leftArea = new QHBoxLayout();
  leftArea->addWidget(workingDir);
  leftArea->addWidget(browseButton);
  connect(browseButton, SIGNAL(clicked()), this, SLOT(openWorkingDirDialog()));

  formLayout->addRow(dirLabel, leftArea);

  QPushButton *makePlanButton = new QPushButton(tr("Make plan"));
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

  Plan *newPlan = new Plan(name, startTime, endTime, daySelect->currentIndex() - 1, workingDirectories);

  emit planCreated(newPlan);
  emit planCreatedMessage(CharacterWords::PLAN_CREATE);
}

void PlanCreateWindow::openWorkingDirDialog()
{
  QFileDialog dirDialog(this);
  dirDialog.setFileMode(QFileDialog::Directory);
  dirDialog.setOption(QFileDialog::ShowDirsOnly);

  if(dirDialog.exec()){
    QStringList dirPaths = dirDialog.selectedFiles();

    for(QString &dir : dirPaths){
        workingDirectories.push_back(QDir(dir));
    }

    workingDir->setText(dirPaths.join("; "));
  }
}
