#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plan.h"
#include "plantime.h"
#include "timenotifier.h"

#include <QtWidgets>
#include <QFrame>
#include <QTimer>
#include <QDebug>
#include <QTime>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  initData();
  setMenu();

  for(int i = 0; i < 7; i++){
    rowFrames[i]->installEventFilter(this);

    rowLabels[i]->setAutoFillBackground(true);
  }

  highlightCurrentDay(QDate::currentDate().dayOfWeek() % 7);
  // mon1~sun7
  qDebug() << "currentDay: " << QDate::currentDate().dayOfWeek();

  connect(ui->addPlanButton, SIGNAL(clicked()), this, SLOT(addPlan()));

  TimeNotifier *timeNotifier = new TimeNotifier();
  ui->apparea->layout()->addWidget(timeNotifier);

  connect(timeNotifier, SIGNAL(dayChanged(int)), this, SLOT(highlightCurrentDay(int)));
  connect(timeNotifier, SIGNAL(minuteChanged(QTime)), this, SLOT(bellProperBell(QTime)));

  Plan *testPlan = new Plan(rowFrames[SUNDAY], "test", new PlanTime(0), new PlanTime(10));
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

  rowFrames[SUNDAY] = ui->sundayFrame;
  rowFrames[MONDAY] = ui->mondayFrame;
  rowFrames[TUESDAY] = ui->tuesdayFrame;
  rowFrames[WEDNESDAY] = ui->wednesdayFrame;
  rowFrames[THIRSDAY] = ui->thirsdayFrame;
  rowFrames[FRIDAY] = ui->fridayFrame;
  rowFrames[SATURDAY] = ui->saturdayFrame;

  rowLabels[SUNDAY] = ui->sundayLabel;
  rowLabels[MONDAY] = ui->mondayLabel;
  rowLabels[TUESDAY] = ui->tuesdayLabel;
  rowLabels[WEDNESDAY] = ui->wednesdayLabel;
  rowLabels[THIRSDAY] = ui->thirsdayLabel;
  rowLabels[FRIDAY] = ui->fridayLabel;
  rowLabels[SATURDAY] = ui->saturdayLabel;

  for(int i = 0; i < 24; i++){
    for(int j = 0; j < 60; j++){
      timeToAlerm[i][j] = NONE_BELL;
    }
  }
}

void MainWindow::setMenu()
{
  QAction *importAction = new QAction(tr("I&mport"), this);
  QAction *exportAction = new QAction(tr("&Export"), this);
  QAction *exitAction = new QAction(tr("E&xit"), this);

  connect(importAction, SIGNAL(triggered()), this, SLOT(importTimetable()));
  connect(exportAction, SIGNAL(triggered()), this, SLOT(exportTimetable()));
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(importAction);
  fileMenu->addAction(exportAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  for(int i = 0; i < 7; i++){
    if(obj == rowFrames[i] && event->type() == QEvent::Resize){
      for(auto plan : timetable[i]){
        plan->updatePlanGeometry();
      }
      return true;
    }
  }

  return false;
}

void MainWindow::importTimetable()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Import File"), "", "JSON Files (*.json)");

  if(!fileName.isEmpty()){
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
      QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
      return;
    }
    QTextStream inStream(&file);
    QString fileText;
    while(!inStream.atEnd()){
      fileText += inStream.readLine() + '\n';
    }

    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileText.toUtf8());

    if(!jsonDoc.isNull()){
      for(int day = 0; day < 7; day++){
        for(auto plan : timetable[day]){
          deletePlan(plan);
        }
      }
      for(int h = 0; h < 24; h++){
        for(int m = 0; m < 60; m++){
          timeToAlerm[h][m] = BellType::NONE_BELL;
        }
      }
      timetable->clear();
      for(int day = 0; day < 7; day++){
        QJsonValue dayInfo = jsonDoc[day];
        QJsonArray dayPlans = dayInfo["plans"].toArray();
        for(auto plan = dayPlans.begin(); plan != dayPlans.end(); plan++){
          QJsonObject planObj = (*plan).toObject();
          QString planName = planObj["planName"].toString();
          PlanTime *startTime = PlanTime::parseTime(planObj["startTime"].toString(), ':');
          PlanTime *endTime = PlanTime::parseTime(planObj["endTime"].toString(), ':');
          Plan *loadedPlan = new Plan(rowFrames[day], planName, startTime, endTime);

          setPlan(loadedPlan, day);
        }
      }

    }
  }
}

void MainWindow::exportTimetable()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export File"), "", "JSON Files (*.json)");

  if(!fileName.isEmpty()){
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
      QMessageBox::critical(this, tr("Error"), tr("Could not open the file."));
      return;
    }
    QJsonArray jsonTimetable;
    QString dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thirsday", "Friday", "Saturday"};
    for(int i = 0; i < 7; i++){
      QJsonObject dayObj;
      dayObj.insert("day", dayNames[i]);

      QJsonArray dayPlans;
      for(int j = 0; j < timetable[i].size(); j++){
        QJsonObject planInfo;
        qDebug() << "capsling " << timetable[i][j]->planName;

        planInfo.insert("planName", timetable[i][j]->planName);
        planInfo.insert("startTime", timetable[i][j]->startTime->toString());
        planInfo.insert("endTime", timetable[i][j]->endTime->toString());

        dayPlans.push_back(planInfo);
      }
      dayObj.insert("plans", dayPlans);

      jsonTimetable.push_back(dayObj);
    }

    QJsonDocument document(jsonTimetable);
    QTextStream outStream(&file);
    outStream << document.toJson(QJsonDocument::Indented);
    file.close();
  }
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

  QWidget *column = rowFrames[dayNum];

  Plan *newPlan = new Plan(column, name, startTime, endTime);
  setPlan(newPlan, dayNum);

}

void MainWindow::setPlan(Plan *newPlan, int dayNum)
{
  newPlan->day = dayNum;
  timetable[dayNum].push_back(newPlan);

  connect(newPlan, SIGNAL(deleteButtonClicked(Plan*)), this, SLOT(deletePlan(Plan*)));
  connect(newPlan, SIGNAL(planClicked(Plan*)), this, SLOT(inspectPlan(Plan*)));

  timeToAlerm[newPlan->startTime->hour][newPlan->startTime->minute] = START_BELL;
  if(timeToAlerm[newPlan->endTime->hour][newPlan->startTime->minute] != START_BELL){
    timeToAlerm[newPlan->endTime->hour][newPlan->startTime->minute] = END_BELL;
  }
  PlanTime *prelimTime = *(newPlan->startTime) - new PlanTime(0, 5);
  if(timeToAlerm[prelimTime->hour][prelimTime->minute] == NONE_BELL){
    timeToAlerm[prelimTime->hour][prelimTime->minute] = PRELIM_BELL;
  }
}

void MainWindow::playBell(QUrl bellPath)
{
  bellPlayer->setMedia(bellPath);
  bellPlayer->setVolume(40);
  bellPlayer->play();
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

void MainWindow::inspectPlan(Plan *plan)
{
  selectedPlan = plan;

  ui->inspectNameLine->setText(plan->planName);
  ui->inspectDayCombo->setCurrentIndex(plan->day);
  ui->inspectStartTime->setTime(QTime(plan->startTime->hour, plan->startTime->minute));
  ui->inspectEndTime->setTime(QTime(plan->endTime->hour, plan->endTime->minute));
}

void MainWindow::highlightCurrentDay(int dayNum)
{
  for(int i = 0; i < 7; i++){
    QPalette plt = palette();

    plt.setColor(QPalette::Background, QColor(248, 251, 254));
    if(i == dayNum){
      plt.setColor(QPalette::Background, QColor(253, 217, 192));
    }

    rowLabels[i]->setPalette(plt);
  }
}

void MainWindow::bellProperBell(QTime currentTime)
{
  switch(timeToAlerm[currentTime.hour()][currentTime.minute()]){
    case NONE_BELL:
      break;
    case START_BELL:
      playBell(QUrl("qrc:/sounds/period_bell.mp3"));
      break;
    case END_BELL:
      playBell(QUrl("qrc:/sounds/period_bell.mp3"));
      break;
    case PRELIM_BELL:
      playBell(QUrl("qrc:/sounds/first_bell.mp3"));
      break;
  }
}

void MainWindow::on_sundayFrame_customContextMenuRequested(const QPoint &pos)
{

}
