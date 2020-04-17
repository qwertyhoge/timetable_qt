#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plan.h"
#include "plantime.h"
#include "timenotifier.h"
#include "characterpanel.h"
#include "daycolumn.h"
#include "timetable.h"

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
  if(!QDir("configs").exists()){
    QDir().mkdir("configs");
  }

  timetable = new Timetable();
  ui->scrollAreaWidgetContents->layout()->addWidget(timetable);
  // ui->scrollArea->setWidget(timetable);

  initData();
  setMenu();

  CharacterPanel *characterPanel = new CharacterPanel();
  ui->apparea->layout()->addWidget(characterPanel);
  connect(characterPanel, SIGNAL(characterClicked()), this, SLOT(openMenu()));

  actionMenu = new ActionMenu(ui->timetableArea);
  connect(actionMenu, SIGNAL(finished(int)), this, SLOT(enableTimetableArea()));

  planCreateWindow = new PlanCreateWindow();
  connect(actionMenu, SIGNAL(createWindowOpen()), this, SLOT(setPlanCreateWindow()));

  for(int i = 0; i < 7; i++){
    rowFrames[i]->installEventFilter(this);

    // rowLabels[i]->setAutoFillBackground(true);
  }

  highlightCurrentDay(QDate::currentDate().dayOfWeek() % 7);
  // mon1~sun7
  qDebug() << "currentDay: " << QDate::currentDate().dayOfWeek();

  connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteSelectedPlan()));
  connect(ui->editCancelButton, SIGNAL(clicked()), this, SLOT(cancelEdit()));

  connect(ui->timeNotifier, SIGNAL(dayChanged(int)), this, SLOT(highlightCurrentDay(int)));
  connect(ui->timeNotifier, SIGNAL(minuteChanged(QTime)), this, SLOT(bellProperBell(QTime)));

  /*
  Plan *testPlan = new Plan(rowFrames[SUNDAY], "test", new PlanTime(0), new PlanTime(10));
  setPlan(testPlan, SUNDAY);
  */

  loadDefaultTimetable();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::initData()
{
  DayColumn *sundayColumn = new DayColumn();
  timetable->layout->addWidget(sundayColumn);
  rowFrames[SUNDAY] = sundayColumn;

  DayColumn *mondayColumn = new DayColumn();
  timetable->layout->addWidget(mondayColumn);
  rowFrames[MONDAY] = mondayColumn;

  DayColumn *tuesdayColumn = new DayColumn();
  timetable->layout->addWidget(tuesdayColumn);
  rowFrames[TUESDAY] = tuesdayColumn;

  DayColumn *wednesdayColumn = new DayColumn();
  timetable->layout->addWidget(wednesdayColumn);
  rowFrames[WEDNESDAY] = wednesdayColumn;

  DayColumn *thirsdayColumn = new DayColumn();
  timetable->layout->addWidget(thirsdayColumn);
  rowFrames[THIRSDAY] = thirsdayColumn;

  DayColumn *fridayColumn = new DayColumn();
  timetable->layout->addWidget(fridayColumn);
  rowFrames[FRIDAY] = fridayColumn;

  DayColumn *saturdayColumn = new DayColumn();
  timetable->layout->addWidget(saturdayColumn);
  rowFrames[SATURDAY] = saturdayColumn;

  /*
  rowLabels[SUNDAY] = sundayColumn->label;
  rowLabels[MONDAY] = ui->mondayLabel;
  rowLabels[TUESDAY] = ui->tuesdayLabel;
  rowLabels[WEDNESDAY] = ui->wednesdayLabel;
  rowLabels[THIRSDAY] = ui->thirsdayLabel;
  rowLabels[FRIDAY] = ui->fridayLabel;
  rowLabels[SATURDAY] = ui->saturdayLabel;
*/

  for(int i = 0; i < 24; i++){
    for(int j = 0; j < 60; j++){
      timeToAlerm[i][j] = NONE_BELL;
    }
  }
}

bool MainWindow::loadDefaultTimetable()
{
  QFile config("./configs/defaulttimetable.cfg");

  if(!config.exists()){
    config.open(QIODevice::WriteOnly);
    config.close();
    return false;
  }

  if(!config.open(QIODevice::ReadOnly)){
    QMessageBox::critical(this, tr("Error"), tr("Could not open the file."));
    return false;
  }
  QString defaultFileName = config.readLine();
  config.close();

  QFile defaultFile(defaultFileName);

  if(defaultFileName.isEmpty() || !defaultFile.exists()){
    qDebug() << "the config is empty or such file does not exist";
    return false;
  }

  if(!defaultFile.open(QIODevice::ReadOnly)){
    QMessageBox::critical(this, tr("Error"), tr("Could not open the file."));
    return false;
  }
  QByteArray contents = defaultFile.readAll();

  loadFromJson(contents);

  openingTimetablePath = defaultFileName;

  return true;
}

void MainWindow::setMenu()
{
  QAction *importAction = new QAction(tr("I&mport"), this);
  QAction *exportAction = new QAction(tr("&Export"), this);
  QAction *setDefaultAction = new QAction(tr("Set as &Default timetable"), this);
  QAction *exitAction = new QAction(tr("E&xit"), this);

  connect(importAction, SIGNAL(triggered()), this, SLOT(importTimetable()));
  connect(exportAction, SIGNAL(triggered()), this, SLOT(exportTimetable()));
  connect(setDefaultAction, SIGNAL(triggered()), this, SLOT(setDefaultTimetable()));
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(importAction);
  fileMenu->addAction(exportAction);
  fileMenu->addAction(setDefaultAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  for(int i = 0; i < 7; i++){
    if(obj == rowFrames[i] && event->type() == QEvent::Resize){
      for(auto plan : timetableData[i]){
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

    loadFromJson(fileText.toUtf8());
    openingTimetablePath = fileName;
  }
}

void MainWindow::loadFromJson(QByteArray json)
{
  QJsonDocument jsonDoc = QJsonDocument::fromJson(json);

  if(!jsonDoc.isNull()){
    for(int day = 0; day < 7; day++){
      for(auto plan : timetableData[day]){
        deletePlan(plan);
      }
    }
    for(int h = 0; h < 24; h++){
      for(int m = 0; m < 60; m++){
        timeToAlerm[h][m] = BellType::NONE_BELL;
      }
    }
    timetableData->clear();
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

void MainWindow::exportTimetable()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export File"), "", "JSON Files (*.json)");

  exportTimetable(fileName);
}

void MainWindow::exportTimetable(QString fileName)
{
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
      for(int j = 0; j < timetableData[i].size(); j++){
        QJsonObject planInfo;
        qDebug() << "capsling " << timetableData[i][j]->planName;

        planInfo.insert("planName", timetableData[i][j]->planName);
        planInfo.insert("startTime", timetableData[i][j]->startTime->toString());
        planInfo.insert("endTime", timetableData[i][j]->endTime->toString());

        dayPlans.push_back(planInfo);
      }
      dayObj.insert("plans", dayPlans);

      jsonTimetable.push_back(dayObj);
    }

    QJsonDocument document(jsonTimetable);
    QTextStream outStream(&file);
    outStream << document.toJson(QJsonDocument::Indented);
    file.close();

    openingTimetablePath = fileName;
  }
}

void MainWindow::setDefaultTimetable()
{
  QFile file("./configs/defaulttimetable.cfg");

  if(openingTimetablePath.isEmpty()){
    QMessageBox::information(this, "Save timetable", tr("Export the timetable first to set it as default."));
    exportTimetable();
  }else{
    exportTimetable(openingTimetablePath);
  }

  if(!file.open(QIODevice::WriteOnly)){
    QMessageBox::critical(this, tr("Error"), tr("Could not open the file."));
    return;
  }
  file.write(openingTimetablePath.toUtf8());
  file.close();
}

void MainWindow::addPlan()
{
  /*
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
  */
}

void MainWindow::setPlan(Plan *newPlan, int dayNum)
{
  newPlan->day = dayNum;
  timetableData[dayNum].push_back(newPlan);

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

  if(!timetableData[plan->day].removeOne(plan)){
    qCritical() << "failed to delete from vector";
  }
  plan->setParent(nullptr);
  plan->deleteLater();
}

void MainWindow::deleteSelectedPlan()
{
  deletePlan(selectedPlan);
  selectedPlan = nullptr;

  ui->inspectNameLine->setText("");
  ui->inspectDayCombo->setCurrentIndex(-1);
  ui->inspectStartTime->setTime(QTime(0, 0));
  ui->inspectEndTime->setTime(QTime(0, 0));

  ui->editButton->setEnabled(false);
  ui->deleteButton->setEnabled(false);
}

void MainWindow::inspectPlan(Plan *plan)
{
  selectedPlan = plan;

  enterInspectMode();

  ui->inspectNameLine->setText(plan->planName);
  ui->inspectDayCombo->setCurrentIndex(plan->day);
  ui->inspectStartTime->setTime(QTime(plan->startTime->hour, plan->startTime->minute));
  ui->inspectEndTime->setTime(QTime(plan->endTime->hour, plan->endTime->minute));
}

void MainWindow::enterInspectMode()
{
  ui->inspectNameLine->setEnabled(false);
  ui->inspectDayCombo->setEnabled(false);
  ui->inspectStartTime->setEnabled(false);
  ui->inspectEndTime->setEnabled(false);

  ui->editButton->setEnabled(true);
  ui->editButton->setText("Edit plan");
  disconnect(ui->editButton, SIGNAL(clicked()), this, SLOT(applyEdit()));
  connect(ui->editButton, SIGNAL(clicked()), this, SLOT(enterEditMode()));

  ui->editCancelButton->setEnabled(false);

  ui->deleteButton->setEnabled(true);
}

void MainWindow::enterEditMode()
{
  ui->inspectNameLine->setEnabled(true);
  ui->inspectDayCombo->setEnabled(true);
  ui->inspectStartTime->setEnabled(true);
  ui->inspectEndTime->setEnabled(true);

  ui->editButton->setEnabled(true);
  ui->editButton->setText("Apply edit");
  disconnect(ui->editButton, SIGNAL(clicked()), this, SLOT(enterEditMode()));
  connect(ui->editButton, SIGNAL(clicked()), this, SLOT(applyEdit()));

  ui->editCancelButton->setEnabled(true);

  ui->deleteButton->setEnabled(false);
}

void MainWindow::cancelEdit()
{
  enterInspectMode();

  ui->inspectNameLine->setText(selectedPlan->planName);
  ui->inspectDayCombo->setCurrentIndex(selectedPlan->day);
  ui->inspectStartTime->setTime(QTime(selectedPlan->startTime->hour, selectedPlan->startTime->minute));
  ui->inspectEndTime->setTime(QTime(selectedPlan->endTime->hour, selectedPlan->endTime->minute));
}

void MainWindow::applyEdit()
{
  // sanitizing may be in need
  selectedPlan->planName = ui->inspectNameLine->text();
  selectedPlan->day = ui->inspectDayCombo->currentIndex();
  selectedPlan->startTime = PlanTime::parseTime(ui->inspectStartTime->text(), ':');
  selectedPlan->endTime = PlanTime::parseTime(ui->inspectEndTime->text(), ':');

  selectedPlan->updateData();

  enterInspectMode();
}

void MainWindow::highlightCurrentDay(int dayNum)
{
  for(int i = 0; i < 7; i++){
    QPalette plt = palette();

    plt.setColor(QPalette::Background, QColor(248, 251, 254));
    if(i == dayNum){
      plt.setColor(QPalette::Background, QColor(253, 217, 192));
    }

   //  rowLabels[i]->setPalette(plt);
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

void MainWindow::openMenu()
{
  QPoint modalAreaCenter = ui->timetableArea->geometry().center() + pos();
  QSize modalAreaSize = QSize(ui->timetableArea->width() * 2 / 3, ui->timetableArea->height() * 2 / 3);
  actionMenu->setMinimumWidth(modalAreaSize.width());
  actionMenu->setMinimumHeight(modalAreaSize.height());
  actionMenu->move(modalAreaCenter.x() - modalAreaSize.width() / 2,
               modalAreaCenter.y() - modalAreaSize.height() / 2);

  actionMenu->show();
  actionMenu->raise();
  actionMenu->activateWindow();
  disableTimetableArea();
}

void MainWindow::setPlanCreateWindow()
{
  planCreateWindow->show();
  planCreateWindow->raise();
  planCreateWindow->activateWindow();
}

void MainWindow::disableTimetableArea()
{
  if(!timetableAreaDisabled){
    ui->timetableArea->setEnabled(false);
    actionMenu->setEnabled(true);

    ui->timetableArea->setStyleSheet("background-color: #cccccc");
    actionMenu->setStyleSheet("background-color: #ffffff");

    timetableAreaDisabled = true;
  }

}
void MainWindow::enableTimetableArea()
{
  if(timetableAreaDisabled){
    ui->timetableArea->setEnabled(true);
    ui->timetableArea->setStyleSheet("");
    actionMenu->setStyleSheet("");

    timetableAreaDisabled = false;
  }
}

void MainWindow::on_sundayFrame_customContextMenuRequested(const QPoint &pos)
{

}
