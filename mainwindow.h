#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "actionmenu.h"
#include "plancreatewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Plan;
class Timetable;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

  void setNewPlan();
private:
  Ui::MainWindow *ui;

  QString openingTimetablePath = "";
  ActionMenu *actionMenu;
  PlanCreateWindow *planCreateWindow;
  Timetable *timetable;
  bool timetableAreaDisabled = false;
  Plan *selectedPlan = nullptr;

  void initData();
  bool loadDefaultTimetable();
  void setMenu();

private slots:
  void importTimetable();
  void exportTimetable();
  void exportTimetable(QString fileName);
  void setDefaultTimetable();
  void addPlan();
  void deleteSelectedPlan();
  void inspectPlan(Plan *plan);
  void enterInspectMode();
  void enterEditMode();
  void cancelEdit();
  void applyEdit();
  void openMenu();
  void setPlanCreateWindow();
  void disableTimetableArea();
  void enableTimetableArea();
};
#endif // MAINWINDOW_H
