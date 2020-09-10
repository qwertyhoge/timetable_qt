#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plantime.h"
#include "timenotifier.h"
#include "characterpanel.h"
#include "timetable.h"
#include "plancreatewindow.h"

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
  connect(timetable, SIGNAL(planClicked(PlanFrame*)), this, SLOT(inspectPlan(PlanFrame*)));
  ui->scrollAreaWidgetContents->layout()->addWidget(timetable);
  // ui->scrollArea->setWidget(timetable);

  initData();
  setMenu();

  characterPanel = new CharacterPanel();
  ui->apparea->layout()->addWidget(characterPanel);
  connect(characterPanel, SIGNAL(characterClicked()), this, SLOT(openMenu()));

  actionMenu = new ActionMenu(this);
  connect(actionMenu, SIGNAL(finished(int)), this, SLOT(enableTimetableArea()));
  connect(actionMenu, SIGNAL(menuQuitMessage(CharacterWords::Timings)), characterPanel, SLOT(processTimings(CharacterWords::Timings)));

  planCreateWindow = new PlanCreateWindow(this);
  connect(actionMenu, SIGNAL(createWindowOpen()), this, SLOT(setPlanCreateWindow()));
  connect(planCreateWindow, SIGNAL(planCreated(Plan*)), timetable, SLOT(addPlan(Plan*)));
  connect(planCreateWindow, SIGNAL(planCreatedMessage(CharacterWords::Timings)), characterPanel, SLOT(processTimings(CharacterWords::Timings)));

  // mon1~sun7
  qDebug() << "currentDay: " << QDate::currentDate().dayOfWeek();

  connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteSelectedPlan()));
  connect(ui->editCancelButton, SIGNAL(clicked()), this, SLOT(cancelEdit()));

  connect(ui->timeNotifier, SIGNAL(minuteChanged(const QDateTime&, bool)), timetable, SLOT(processPlanTimings(const QDateTime&, bool)));
  connect(timetable, SIGNAL(planStartedMessage(CharacterWords::Timings)), characterPanel, SLOT(processTimings(CharacterWords::Timings)));
  connect(timetable, SIGNAL(planEndedMessage(CharacterWords::Timings)), characterPanel, SLOT(processTimings(CharacterWords::Timings)));
  connect(timetable, SIGNAL(planPrelimMessage(CharacterWords::Timings)), characterPanel, SLOT(processTimings(CharacterWords::Timings)));

  loadDefaultTimetable();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::initData()
{
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

  timetable->loadFromJson(contents);

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


void MainWindow::importTimetable()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Import File"), "", "JSON Files (*.json)");

  if(!fileName.isEmpty()){
    importTimetable(fileName);
  }

}

void MainWindow::importTimetable(const QString fileName)
{

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

  timetable->loadFromJson(fileText.toUtf8());
  openingTimetablePath = fileName;
}

void MainWindow::exportTimetable()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export File"), "", "JSON Files (*.json)");

  if(!fileName.isEmpty()){
    exportTimetable(fileName);
    }
}

void MainWindow::exportTimetable(const QString fileName)
{
  QFile file(fileName);
  if(!file.open(QIODevice::WriteOnly)){
    QMessageBox::critical(this, tr("Error"), tr("Could not open the file."));
    return;
  }

  QJsonArray jsonTimetable = timetable->exportAsJson();

  QJsonDocument document(jsonTimetable);
  QTextStream outStream(&file);
  outStream << document.toJson(QJsonDocument::Indented);
  file.close();

  openingTimetablePath = fileName;
}

void MainWindow::setDefaultTimetable()
{
  QFile file("./configs/defaulttimetable.cfg");

  if(openingTimetablePath.isEmpty()){
    QMessageBox::information(this, "Save timetable", tr("Export the timetable once to set as default."));
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

void MainWindow::deleteSelectedPlan()
{
  timetable->deletePlanFrame(selectedPlanFrame);
  selectedPlanFrame = nullptr;

  ui->inspectNameLine->setText("");
  ui->inspectDayCombo->setCurrentIndex(-1);
  ui->inspectStartTime->setTime(QTime(0, 0));
  ui->inspectEndTime->setTime(QTime(0, 0));

  ui->editButton->setEnabled(false);
  ui->deleteButton->setEnabled(false);

  // this must be moved to connected slot after moving delete button from ui to source
  characterPanel->processTimings(CharacterWords::PLAN_DELETE);
}

void MainWindow::inspectPlan(PlanFrame *plan)
{
  selectedPlanFrame = plan;
  Plan *planData = selectedPlanFrame->getPlanData();

  enterInspectMode();

  ui->inspectNameLine->setText(planData->getPlanName());
  ui->inspectDayCombo->setCurrentIndex(planData->getDayNum());
  ui->inspectStartTime->setTime(planData->getStartTime().toQTime());
  ui->inspectEndTime->setTime(planData->getEndTime().toQTime());

  ui->inspectDirLine->setText(planData->dirsAsString());
}

void MainWindow::enterInspectMode()
{
  ui->inspectNameLine->setEnabled(false);
  ui->inspectDayCombo->setEnabled(false);
  ui->inspectStartTime->setEnabled(false);
  ui->inspectEndTime->setEnabled(false);

  ui->editButton->setEnabled(true);
  ui->editButton->setText("Edit plan");
  disconnect(ui->editButton, SIGNAL(clicked()), nullptr, nullptr);
  connect(ui->editButton, SIGNAL(clicked()), this, SLOT(enterEditMode()));
  // connect(ui->editButton, SIGNAL(clicked()), characterPanel, SLOT(showPlanEditStartMessage()));

  ui->editCancelButton->setEnabled(false);

  ui->deleteButton->setEnabled(true);
}

void MainWindow::enterEditMode()
{
  // connect after move from ui to source
  characterPanel->processTimings(CharacterWords::PLAN_EDIT_START);

  ui->inspectNameLine->setEnabled(true);
  ui->inspectDayCombo->setEnabled(true);
  ui->inspectStartTime->setEnabled(true);
  ui->inspectEndTime->setEnabled(true);

  ui->editButton->setEnabled(true);
  ui->editButton->setText("Apply edit");
  disconnect(ui->editButton, SIGNAL(clicked()), nullptr, nullptr);
  connect(ui->editButton, SIGNAL(clicked()), this, SLOT(applyEdit()));
  // connect(ui->editButton, SIGNAL(clicked()), characterPanel, SLOT(showPlanEditDoneMessage()));
  qDebug() << characterPanel;

  ui->editCancelButton->setEnabled(true);

  ui->deleteButton->setEnabled(false);
}

void MainWindow::cancelEdit()
{
  enterInspectMode();

  Plan *planData = selectedPlanFrame->getPlanData();

  ui->inspectNameLine->setText(planData->getPlanName());
  ui->inspectDayCombo->setCurrentIndex(planData->getDayNum());
  ui->inspectStartTime->setTime(planData->getStartTime().toQTime());
  ui->inspectEndTime->setTime(planData->getEndTime().toQTime());

  // this must be moved to connected slot after moving cancel button from ui to source

  characterPanel->processTimings(CharacterWords::PLAN_EDIT_CANCEL);
}

void MainWindow::applyEdit()
{
  // connect after move from ui to source
  characterPanel->processTimings(CharacterWords::PLAN_EDIT_DONE);

  // sanitizing may be in need
  QString name = ui->inspectNameLine->text();
  // dayCombo must let the user choose completely the same numbers as DayNums
  DayConsts::DayNums dayNum = DayConsts::intToDayNums(ui->inspectDayCombo->currentIndex());
  PlanTime *start = PlanTime::parseTime(ui->inspectStartTime->text(), ':');
  PlanTime *end = PlanTime::parseTime(ui->inspectEndTime->text(), ':');
  QVector<QDir> dirs;
  for(auto path : selectedPlanFrame->getPlanData()->dirsAsString().split(';')){
    QDir dir(path);
    dirs.push_back(dir);
  }

  Plan *edittedPlan = new Plan(name, start, end, dayNum, dirs);

  selectedPlanFrame->attachPlan(edittedPlan);

  enterInspectMode();
}

void MainWindow::openMenu()
{
  characterPanel->processTimings(CharacterWords::MENU_OPEN);

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

    ui->timetableArea->setStyleSheet("background: #cccccc");

    timetableAreaDisabled = true;
  }

}

void MainWindow::enableTimetableArea()
{
  if(timetableAreaDisabled){
    ui->timetableArea->setEnabled(true);
    ui->timetableArea->setStyleSheet("");

    timetableAreaDisabled = false;
  }
}
