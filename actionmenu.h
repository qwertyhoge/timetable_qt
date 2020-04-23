#ifndef ACTIONMENU_H
#define ACTIONMENU_H

#include <QDialog>
#include <QVBoxLayout>

class ActionMenu : public QDialog
{
  Q_OBJECT

public:
  ActionMenu(QWidget *parent = nullptr);

private:
  QVBoxLayout *layout;
  QPushButton *addPlanButton;
  QPushButton *deletePlanButton;
  QPushButton *quitButton;

private slots:
  void closeMenu();
  void startAdd();
  void startDelete();

signals:
  void deleteStarted();
  void createWindowOpen();

};

#endif // ACTIONMENU_H
