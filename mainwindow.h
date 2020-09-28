#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "actionmenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Timetable;
class CharacterPanel;
class PlanCreateWindow;
class PlanFrame;
class Plan;
class PlanInspectForm;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

private:
  Ui::MainWindow *ui;

  QString openingTimetablePath = "";
  ActionMenu *actionMenu;
  PlanCreateWindow *planCreateWindow;
  Timetable *timetable;
  CharacterPanel *characterPanel;
  PlanInspectForm *planInspectForm;

  bool timetableAreaDisabled = false;
  PlanFrame *selectedPlanFrame = nullptr;

  void initData();
  bool loadDefaultTimetable();
  void setMenu();

private slots:
  void importTimetable();
  void importTimetable(const QString fileName);
  void exportTimetable();
  void exportTimetable(const QString fileName);
  void setDefaultTimetable();
  void selectPlanFrame(PlanFrame *clicked);
  void deleteSelectedPlan();
  void applyEdit(Plan *edittedPlan);
  void openMenu();
  void setPlanCreateWindow();
  void disableTimetableArea();
  void enableTimetableArea();
};
#endif // MAINWINDOW_H
