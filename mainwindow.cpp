#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plan.h"
#include "plantime.h"
#include "timenotifier.h"

#include <QFrame>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  initData();

  for(int i = 0; i < 7; i++){
    columnFrames[i]->installEventFilter(this);

    columnLabels[i]->setAutoFillBackground(true);
  }

  highlightCurrentDay(QDate::currentDate().day() - 1);
  // sun1~satur7?
  qDebug() << "currentDay: " << QDate::currentDate().day();

  connect(ui->addPlanButton, SIGNAL(clicked()), this, SLOT(addPlan()));

  TimeNotifier *timeNotifier = new TimeNotifier();
  ui->apparea->layout()->addWidget(timeNotifier);

  connect(timeNotifier, SIGNAL(dayChanged(int)), this, SLOT(highlightCurrentDay(int)));

  Plan *testPlan = new Plan(columnFrames[SUNDAY], "test", new PlanTime(0), new PlanTime(10));
  setPlan(testPlan, SUNDAY);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::initData()
{
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

  columnLabels[SUNDAY] = ui->sundayLabel;
  columnLabels[MONDAY] = ui->mondayLabel;
  columnLabels[TUESDAY] = ui->tuesdayLabel;
  columnLabels[WEDNESDAY] = ui->wednesdayLabel;
  columnLabels[THIRSDAY] = ui->thirsdayLabel;
  columnLabels[FRIDAY] = ui->fridayLabel;
  columnLabels[SATURDAY] = ui->saturdayLabel;

  for(int i = 0; i < 24; i++){
    for(int j = 0; j < 60; j++){
      timeToAlerm[i][j] = NONE_BELL;
    }
  }
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

void MainWindow::highlightCurrentDay(int dayNum)
{
  for(int i = 0; i < 7; i++){
    QPalette plt = palette();

    plt.setColor(QPalette::Background, QColor(248, 251, 254));
    if(i == dayNum){
      plt.setColor(QPalette::Background, QColor(253, 217, 192));
    }

    columnLabels[i]->setPalette(plt);
  }
}

void MainWindow::on_sundayFrame_customContextMenuRequested(const QPoint &pos)
{

}
