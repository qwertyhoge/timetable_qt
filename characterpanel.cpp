#include "characterpanel.h"

#include "characterview.h"
#include "replybox.h"

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
  connect(characterArea, SIGNAL(characterClicked()), this, SLOT(sendMenuOpen()));

  textArea = new QTextEdit();
  layout->addWidget(textArea);
  layout->setStretch(1, 1);

  replyBox = new ReplyBox();

  setWidget(content);
}

void CharacterPanel::speakWord(CharacterWords::Timings timing)
{
  currentWord = characterWords->pickRandomOne(timing);

  setConvo();
}

void CharacterPanel::setConvo()
{
  QString escapedStr = currentWord->sentence.replace("\\n", "\n");
  textArea->setText(escapedStr);

  if(currentWord->hasConvo()){
    widget()->layout()->addWidget(replyBox);

    disconnect(replyBox, SIGNAL(yesClicked()), nullptr, nullptr);
    disconnect(replyBox, SIGNAL(noClicked()), nullptr, nullptr);

    connect(replyBox, SIGNAL(yesClicked()), this, SLOT(yesReply()));
    connect(replyBox, SIGNAL(noClicked()), this, SLOT(noReply()));
  }else{
    replyBox->setParent(nullptr);
  }
}

void CharacterPanel::sendMenuOpen()
{
  showMenuOpenMessage();

  // propagate signal
  emit characterClicked();
}

void CharacterPanel::yesReply()
{
  currentWord = currentWord->convoYes;

  setConvo();
}

void CharacterPanel::noReply()
{
  currentWord = currentWord->convoNo;

  setConvo();
}

void CharacterPanel::showRunMessage()
{
  speakWord(CharacterWords::RUN);
}

void CharacterPanel::showMenuOpenMessage()
{
  speakWord(CharacterWords::MENU_OPEN);
}

void CharacterPanel::showMenuQuitMessage()
{
  speakWord(CharacterWords::MENU_QUIT);
}

void CharacterPanel::showPlanEditStartMessage()
{
  speakWord(CharacterWords::PLAN_EDIT_START);
}

void CharacterPanel::showPlanEditCancelMessage()
{
  speakWord(CharacterWords::PLAN_EDIT_CANCEL);
}

void CharacterPanel::showPlanEditDoneMessage()
{
  speakWord(CharacterWords::PLAN_EDIT_DONE);
}

void CharacterPanel::showPlanCreateMessage()
{
  speakWord(CharacterWords::PLAN_CREATE);
}

void CharacterPanel::showPlanDeleteMessage()
{
  speakWord(CharacterWords::PLAN_DELETE);
}
