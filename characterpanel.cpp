#include "characterpanel.h"

#include "characterview.h"

#include <QLayout>
#include <QTextEdit>
#include <QDebug>

CharacterPanel::CharacterPanel(QWidget *parent)
  : QDockWidget(parent)
{
  QWidget *content = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout();
  content->setLayout(layout);

  characterArea = new CharacterView();
  layout->addWidget(characterArea);
  connect(characterArea, SIGNAL(characterClicked()), this, SLOT(showMenuMessage()));

  textArea = new QTextEdit();
  layout->addWidget(textArea);
  layout->setStretch(1, 1);

  setWidget(content);

  qDebug() << widget();
  qDebug() << content;
}

void CharacterPanel::showMenuMessage()
{

  // propagate signal
  emit characterClicked();
}
