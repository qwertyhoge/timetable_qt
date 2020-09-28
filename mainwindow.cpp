#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "plantime.h"
#include "timenotifier.h"
#include "characterpanel.h"
#include "timetable.h"
#include "plancreatewindow.h"
#include "planinspectform.h"

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
  connect(timetable, SIGNAL(planClicked(PlanFrame*)), this, SLOT(selectPlanFrame(PlanFrame*)));
  ui->scrollAreaWidgetContents->layout()->addWidget(timetable);
  // ui->scrollArea->setWidget(timetable);

  initData();
  setMenu();

  planInspectForm = new PlanInspectForm();
  ui->headerLayout->addWidget(planInspectForm);
  connect(planInspectForm, SIGNAL(currentPlanDeleteQuery()), this, SLOT(deleteSelectedPlan()));
  connect(planInspectForm, SIGNAL(currentPlanChangeQuery(Plan*)), this, SLOT(applyEdit(Plan*)));

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

  connect(ui->timeNotifier, SIGNAL(minuteChanged(const QTime&, DayConsts::DayNums, bool)), timetable, SLOT(processPlanTimings(const QTime&, DayConsts::DayNums, bool)));
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

void MainWindow::selectPlanFrame(PlanFrame *clicked)
{
  selectedPlanFrame = clicked;

  planInspectForm->inspectPlan(selectedPlanFrame->getPlanData());
}

// should be called by query from the inspect form
void MainWindow::deleteSelectedPlan()
{
  timetable->deletePlanFrame(selectedPlanFrame);
  selectedPlanFrame = nullptr;

  // this must be moved to connected slot after moving delete button from ui to source
  characterPanel->processTimings(CharacterWords::PLAN_DELETE);
}

void MainWindow::applyEdit(Plan *edittedPlan)
{
  // connect after move from ui to source
  characterPanel->processTimings(CharacterWords::PLAN_EDIT_DONE);

  if(edittedPlan->getDayNum() == ui->timeNotifier->getCurrentDay()){
    if(!timetable->switchPlanRegistration(selectedPlanFrame->getPlanData(), edittedPlan)){
      qCritical() << "failed to remove plan registration";
    }
  }

  selectedPlanFrame->attachPlan(edittedPlan);
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
