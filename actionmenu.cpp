#include "actionmenu.h"
#include "plancreatewindow.h"

#include <QPushButton>

ActionMenu::ActionMenu(QWidget *parent)
  : QDialog(parent)
{
  layout = new QVBoxLayout();
  setLayout(layout);
  layout->addStretch(1);

  addPlanButton = new QPushButton(tr("Add plan"));
  layout->addWidget(addPlanButton);
  layout->addStretch(1);
  connect(addPlanButton, SIGNAL(clicked()), this, SLOT(startAdd()));

  deletePlanButton = new QPushButton(tr("Delete plans"));
  layout->addWidget(deletePlanButton);
  layout->addStretch(1);
  connect(deletePlanButton, SIGNAL(clicked()), this, SLOT(startDelete()));

  quitButton = new QPushButton(tr("Quit menu"));
  layout->addWidget(quitButton);
  layout->addStretch(1);
  connect(quitButton, SIGNAL(clicked()), this, SLOT(closeMenu()));
}

void ActionMenu::closeMenu()
{
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
