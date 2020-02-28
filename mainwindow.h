#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMap>

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

  static const int SUNDAY = 0;
  static const int MONDAY = 1;
  static const int TUESDAY = 2;
  static const int WEDNESDAY = 3;
  static const int THIRSDAY = 4;
  static const int FRIDAY = 5;
  static const int SATURDAY = 6;

  QMap<QString, int> dayMap;

  void setNewPlan();
private:
  Ui::MainWindow *ui;

  QWidget* columnFrames[7];
  QVector<Plan*> timetable[7];

  void setPlan(Plan *newPlan, int dayNum);
protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void addPlan();
  void deletePlan(Plan *plan);

  void on_sundayFrame_customContextMenuRequested(const QPoint &pos);
};
#endif // MAINWINDOW_H
