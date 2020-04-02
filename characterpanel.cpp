#include "characterpanel.h"

#include "characterview.h"

#include <QLayout>
#include <QTextEdit>
#include <QDebug>

CharacterPanel::CharacterPanel(QWidget *parent)
  : QWidget(parent)
{
  QHBoxLayout *layout = new QHBoxLayout();
  setLayout(layout);

  textArea = new QTextEdit();
  layout->addWidget(textArea);
  layout->setStretch(0, 1);

  characterArea = new CharacterView();
  layout->addWidget(characterArea);

}
