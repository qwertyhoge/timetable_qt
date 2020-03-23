#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMap>
#include <QLabel>
#include <QTime>
#include <QMediaPlayer>

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

  enum BellType{
    NONE_BELL,
    START_BELL,
    END_BELL,
    PRELIM_BELL
  };

  QWidget *rowFrames[7];
  QLabel *rowLabels[7];
  QVector<Plan*> timetable[7];
  BellType timeToAlerm[24][60];
  Plan *selectedPlan = nullptr;
  QString openingTimetablePath = "";

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
  void highlightCurrentDay(int day);
  void bellProperBell(QTime currentTime);

  void on_sundayFrame_customContextMenuRequested(const QPoint &pos);
};
#endif // MAINWINDOW_H
