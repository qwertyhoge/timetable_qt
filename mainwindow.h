#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMap>
#include <QLabel>
#include <QTime>
#include <QMediaPlayer>

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

  enum BellType{
    NONE_BELL,
    START_BELL,
    END_BELL,
    PRELIM_BELL
  };
  enum Days{
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THIRSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6
  };
  static QMap<QString, int> dayMap;
  QFrame *rowFrames[7];

  QLabel *rowLabels[7];
  QVector<Plan*> timetableData[7];
  BellType timeToAlerm[24][60];
  Plan *selectedPlan = nullptr;
  QString openingTimetablePath = "";
  ActionMenu *actionMenu;
  PlanCreateWindow *planCreateWindow;
  Timetable *timetable;
  bool timetableAreaDisabled = false;

  void initData();
  bool loadDefaultTimetable();
  void setMenu();
  void setPlan(Plan *newPlan, int dayNum);
  void playBell(QUrl bellPath);
  void loadFromJson(QByteArray json);

  QMediaPlayer *bellPlayer = new QMediaPlayer();
protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void importTimetable();
  void exportTimetable();
  void exportTimetable(QString fileName);
  void setDefaultTimetable();
  void addPlan();
  void deletePlan(Plan *plan);
  void deleteSelectedPlan();
  void inspectPlan(Plan *plan);
  void enterInspectMode();
  void enterEditMode();
  void cancelEdit();
  void applyEdit();
  void highlightCurrentDay(int day);
  void bellProperBell(QTime currentTime);
  void openMenu();
  void setPlanCreateWindow();
  void disableTimetableArea();
  void enableTimetableArea();

  void on_sundayFrame_customContextMenuRequested(const QPoint &pos);
};
#endif // MAINWINDOW_H
