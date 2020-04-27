#include "characterpanel.h"

#include "characterview.h"
#include "characterwords.h"

#include <QLayout>
#include <QTextEdit>
#include <QDebug>

CharacterPanel::CharacterPanel(QWidget *parent)
  : QDockWidget(parent)
{
  characterWords = new CharacterWords();
  if(!characterWords->loadWords()){
    qCritical() << "failed to load words";
  }

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
  //characterWords->
  textArea->setText("");

  // propagate signal
  emit characterClicked();
}
