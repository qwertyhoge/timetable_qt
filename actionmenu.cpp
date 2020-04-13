#include "actionmenu.h"

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

  deletePlanButton = new QPushButton(tr("Delete Button"));
  layout->addWidget(deletePlanButton);
  layout->addStretch(1);

  quitButton = new QPushButton(tr("Quit menu"));
  layout->addWidget(quitButton);
  layout->addStretch(1);
}
