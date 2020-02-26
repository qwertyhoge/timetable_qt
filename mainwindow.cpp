#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plan.h"
#include "plantime.h"

#include <QFrame>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  dayMap = {
    {"Sunday", 0},
    {"Monday", 1},
    {"Tuesday", 2},
    {"Wednesday", 3},
    {"Thirsday", 4},
    {"Friday", 5},
    {"Saturday", 6},
  };

  columnFrames[SUNDAY] = ui->sundayFrame;
  columnFrames[MONDAY] = ui->mondayFrame;
  columnFrames[TUESDAY] = ui->tuesdayFrame;
  columnFrames[WEDNESDAY] = ui->wednesdayFrame;
  columnFrames[THIRSDAY] = ui->thirsdayFrame;
  columnFrames[FRIDAY] = ui->fridayFrame;
  columnFrames[SATURDAY] = ui->saturdayFrame;

  for(int i = 0; i < 7; i++){
    columnFrames[i]->installEventFilter(this);
  }

  connect(ui->addPlanButton, SIGNAL(clicked()), this, SLOT(addPlan()));

  Plan *testPlan = new Plan(columnFrames[SUNDAY], "test", new PlanTime(0), new PlanTime(10));
  setPlan(testPlan, SUNDAY);
}

MainWindow::~MainWindow()
{
  delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  for(int i = 0; i < 7; i++){
    if(obj == columnFrames[i] && event->type() == QEvent::Resize){
      for(auto plan : timetable[i]){
        plan->updatePlanGeometry();
      }
      return true;
    }
  }

  return false;
}

void MainWindow::addPlan()
{
  QString name = ui->formNameLine->text();
  QString startTimeText = ui->formStartTime->text();
  QString endTimeText = ui->formEndTime->text();
  QString day = ui->formDayCombo->currentText();

  if(day == ""){
    // error
    return;
  }

  PlanTime *startTime = PlanTime::parseTime(startTimeText, ':');
  PlanTime *endTime  = PlanTime::parseTime(endTimeText, ':');
  int dayNum = dayMap[day];

  QWidget *column = columnFrames[dayNum];

  Plan *newPlan = new Plan(column, name, startTime, endTime);
  setPlan(newPlan, dayNum);
}

void MainWindow::setPlan(Plan *newPlan, int dayNum)
{
  newPlan->day = dayNum;
  timetable[dayNum].push_back(newPlan);

  connect(newPlan, SIGNAL(deleteButtonClicked(Plan*)), this, SLOT(deletePlan(Plan*)));
}

void MainWindow::deletePlan(Plan *plan)
{
  qDebug() << "remove";

  if(!timetable[plan->day].removeOne(plan)){
    qCritical() << "failed to delete from vector";
  }
  plan->setParent(nullptr);
  plan->deleteLater();
}

void MainWindow::on_sundayFrame_customContextMenuRequested(const QPoint &pos)
{

}
