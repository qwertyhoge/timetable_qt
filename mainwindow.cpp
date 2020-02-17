#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plan.h"
#include "plantime.h"

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

  Plan *testPlan = new Plan(columnFrames[SUNDAY], "test", PlanTime(0), PlanTime(10));

  testPlan->setGeometry(0, 0, 100, 200);
}

MainWindow::~MainWindow()
{
  delete ui;
}

