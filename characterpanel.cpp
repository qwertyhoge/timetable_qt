#include "characterpanel.h"

#include "characterview.h"
#include "replybox.h"

#include "characterwords.h"

#include <QLayout>
#include <QTextEdit>
#include <QTimer>

#include <QPropertyAnimation>

#include <QDebug>

CharacterPanel::CharacterPanel(QWidget *parent)
  : QDockWidget(parent)
{
  speakTimer = new QTimer();

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
  characterArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

  convoArea = new QWidget();
  layout->addWidget(convoArea);
  convoArea->installEventFilter(this);

  textArea = new QTextEdit(convoArea);
  textArea->move(0, 0);
  textArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  textArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  replyBox = new ReplyBox();

  setWidget(content);
}

void CharacterPanel::speakWord(CharacterWords::Timings timing)
{
  QPropertyAnimation *anim = new QPropertyAnimation(textArea, "geometry");

  anim->setDuration(600);
  anim->setEasingCurve(QEasingCurve::InOutQuart);
  anim->setStartValue(textArea->geometry());
  anim->setEndValue(QRect(0, 0, convoArea->width(), this->height() / 4));
  qDebug() << convoArea->width();
  qDebug() << this->width();

  anim->start();

  currentWord = characterWords->pickRandomOne(timing);

  if(currentWord != nullptr){
    setConvo();
  }
}

void CharacterPanel::collapseMessage()
{
  QPropertyAnimation *anim = new QPropertyAnimation(textArea, "geometry");

  qDebug() << "collapse";
  anim->setDuration(600);
  anim->setEasingCurve(QEasingCurve::InOutQuart);
  anim->setStartValue(textArea->geometry());
  anim->setEndValue(QRect(0, 0, 0, 0));

  anim->start();
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

    speakTimer->setSingleShot(true);
    speakTimer->setInterval(600 + showMessageMSec);
    speakTimer->start();
    connect(speakTimer, SIGNAL(timeout()), this, SLOT(collapseMessage()));
  }
}

bool CharacterPanel::eventFilter(QObject *obj, QEvent *event)
{
  if(obj == convoArea && event->type() == QEvent::Show){
    speakWord(CharacterWords::RUN);
    return true;
  }

  return false;
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
