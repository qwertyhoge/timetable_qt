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

  Plan *testPlan = new Plan(columnFrames[SUNDAY], "test", PlanTime(0), PlanTime(10));
  timetable[SUNDAY].push_back(testPlan);
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

void MainWindow::on_sundayFrame_customContextMenuRequested(const QPoint &pos)
{

}
