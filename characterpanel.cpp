#include "characterpanel.h"

#include "characterview.h"

#include <QLayout>
#include <QTextEdit>
#include <QDebug>

CharacterPanel::CharacterPanel(QWidget *parent)
  : QDockWidget(parent)
{
  QWidget *content = new QWidget(this);
  QHBoxLayout *layout = new QHBoxLayout();
  content->setLayout(layout);

  textArea = new QTextEdit();
  layout->addWidget(textArea);
  layout->setStretch(0, 1);

  characterArea = new CharacterView();
  layout->addWidget(characterArea);

  setWidget(content);

  qDebug() << widget();
  qDebug() << content;
}
