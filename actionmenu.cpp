#include "actionmenu.h"

#include <QPushButton>

ActionMenu::ActionMenu(QWidget *parent)
  : QDialog(parent)
{
  unsigned int flags = windowFlags();
  flags = flags & (~Qt::WindowContextHelpButtonHint);
  flags |= Qt::FramelessWindowHint;
  setWindowFlags(Qt::WindowFlags(flags));

  setStyleSheet("ActionMenu{\
                  border: 5px double #b3dfc6;\
                  background: #f8fcfe\
                }");
  layout = new QVBoxLayout();
  setLayout(layout);

  addPlanButton = new QPushButton(tr("Add plan"));
  layout->addWidget(addPlanButton);
  connect(addPlanButton, SIGNAL(clicked()), this, SLOT(startAdd()));

  deletePlanButton = new QPushButton(tr("Delete plans"));
  layout->addWidget(deletePlanButton);
  connect(deletePlanButton, SIGNAL(clicked()), this, SLOT(startDelete()));

  quitButton = new QPushButton(tr("Quit menu"));
  layout->addWidget(quitButton);
  connect(quitButton, SIGNAL(clicked()), this, SLOT(closeMenu()));
}

void ActionMenu::closeMenu()
{
  emit menuQuitMessage(CharacterWords::MENU_QUIT);
  close();
}

void ActionMenu::startAdd()
{
  emit createWindowOpen();
  close();
}

void ActionMenu::startDelete()
{
  emit deleteStarted();
}
