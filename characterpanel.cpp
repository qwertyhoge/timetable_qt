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
  closeTimer = new QTimer();
  closeTimer->setSingleShot(true);
  closeTimer->setInterval(showMessageMSec);
  connect(closeTimer, SIGNAL(timeout()), this, SLOT(collapseBox()));

  speakTimer = new QTimer();
  speakTimer->setInterval(charInterval);
  connect(speakTimer, SIGNAL(timeout()), this, SLOT(speakCharByChar()));

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
  textArea->resize(0, 0);
  textArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  textArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  openAnim = new QPropertyAnimation(textArea, "geometry");
  connect(openAnim, SIGNAL(valueChanged(const QVariant&)), this, SLOT(watchBoxAppeared(const QVariant&)));
  closeAnim = new QPropertyAnimation(textArea, "geometry");
  connect(closeAnim, SIGNAL(valueChanged(const QVariant&)), this, SLOT(watchBoxDisappeared(const QVariant&)));

  replyBox = new ReplyBox();

  setWidget(content);
}

void CharacterPanel::speakWord(CharacterWords::Timings timing)
{
  qDebug() << "speakWord";
  qDebug() << currentState;

  if(characterWords->hasTree(timing)){
    currentWord = characterWords->pickRandomOne(timing);

    if(currentState == HIDDEN || currentState == CLOSING){
      closeAnim->stop();
      openAnim->setDuration(600);
      openAnim->setEasingCurve(QEasingCurve::InOutQuart);
      openAnim->setStartValue(textArea->geometry());
      openAnim->setEndValue(QRect(0, 0, convoArea->width(), this->height() / 4));
      qDebug() << convoArea->width();
      qDebug() << this->width();

      currentState = OPENING;
      // call startSpeak() after this animation
      openAnim->start();
    }else if(currentState == OPENED){
      closeTimer->stop();
      startSpeak();
    }
  }
}

void CharacterPanel::collapseBox()
{
  qDebug() << "collapse";

  textArea->setText("");

  closeAnim->setDuration(600);
  closeAnim->setEasingCurve(QEasingCurve::InOutQuart);
  closeAnim->setStartValue(textArea->geometry());
  closeAnim->setEndValue(QRect(0, 0, 0, 0));

  closeAnim->start();
  currentState = CLOSING;
}

void CharacterPanel::watchBoxAppeared(const QVariant& animVal)
{
  if(animVal == openAnim->endValue()){
    currentState = OPENED;
    qDebug() << "completely appeared";
    startSpeak();
  }
}

void CharacterPanel::watchBoxDisappeared(const QVariant& animVal)
{
  if(animVal == closeAnim->endValue()){
    currentState = HIDDEN;
  }
}

void CharacterPanel::startSpeak()
{
  QString escapedStr = currentWord->sentence.replace("\\n", "\n");

  replyBox->setParent(nullptr);

  wordQue.clear();
  for(auto ch = escapedStr.begin(); ch !=escapedStr.end(); ch++){
    wordQue.enqueue(*ch);
  }
  textArea->setText("");

  speakTimer->start();
  qDebug() << "started timer";
}

void CharacterPanel::speakCharByChar()
{
  if(!wordQue.empty()){
    textArea->setText(textArea->toPlainText() + wordQue.dequeue());
  }else{
    // end this speak loop itself
    speakTimer->stop();

    setConvo();
  }
}

void CharacterPanel::setConvo()
{
  if(currentWord->hasConvo()){
    widget()->layout()->addWidget(replyBox);

    disconnect(replyBox, SIGNAL(yesClicked()), nullptr, nullptr);
    disconnect(replyBox, SIGNAL(noClicked()), nullptr, nullptr);

    connect(replyBox, SIGNAL(yesClicked()), this, SLOT(yesReply()));
    connect(replyBox, SIGNAL(noClicked()), this, SLOT(noReply()));
  }else{
    closeTimer->start();
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

  startSpeak();
}

void CharacterPanel::noReply()
{
  currentWord = currentWord->convoNo;

  startSpeak();
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

void CharacterPanel::showPlanStartMessage()
{
  speakWord(CharacterWords::PLAN_START);
}
