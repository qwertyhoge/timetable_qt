#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMap>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Plan;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

  enum Days{
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THIRSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6
  };

  QMap<QString, int> dayMap;

  void setNewPlan();
private:
  Ui::MainWindow *ui;

  enum AlermType{
    NONE_BELL,
    START_BELL,
    END_BELL,
    PRELIM_BELL
  };

  QWidget *columnFrames[7];
  QLabel *columnLabels[7];
  QVector<Plan*> timetable[7];
  AlermType timeToAlerm[24][60];

  void initData();
  void setPlan(Plan *newPlan, int dayNum);
protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void addPlan();
  void deletePlan(Plan *plan);
  void highlightCurrentDay(int day);

  void on_sundayFrame_customContextMenuRequested(const QPoint &pos);
};
#endif // MAINWINDOW_H
