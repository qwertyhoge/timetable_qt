#ifndef ACTIONMENU_H
#define ACTIONMENU_H

#include <QDialog>
#include <QLayout>

class ActionMenu : public QDialog
{
public:
  ActionMenu(QWidget *parent = nullptr);

private:
  QVBoxLayout *layout;
  QPushButton *addPlanButton;
  QPushButton *deletePlanButton;
  QPushButton *quitButton;

public slots:

signals:


};

#endif // ACTIONMENU_H
