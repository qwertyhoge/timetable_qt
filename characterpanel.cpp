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
  boxTimer = new QTimer();
  boxTimer->setSingleShot(true);
  boxTimer->setInterval(showMessageMSec);
  connect(boxTimer, SIGNAL(timeout()), this, SLOT(collapseBox()));

  speakTimer = new QTimer();
  speakTimer->setInterval(charInterval);
  connect(speakTimer, SIGNAL(timeout()), this, SLOT(speakCharByChar()));

  connect(this, SIGNAL(speakEnd()), this, SLOT(setConvo()));

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

  boxAnim = new QPropertyAnimation(textArea, "geometry");

  replyBox = new ReplyBox();

  setWidget(content);
}

void CharacterPanel::speakWord(CharacterWords::Timings timing)
{
  boxAnim->setDuration(600);
  boxAnim->setEasingCurve(QEasingCurve::InOutQuart);
  boxAnim->setStartValue(textArea->geometry());
  boxAnim->setEndValue(QRect(0, 0, convoArea->width(), this->height() / 4));
  qDebug() << convoArea->width();
  qDebug() << this->width();

  boxAnim->start();

  connect(boxAnim, SIGNAL(valueChanged(const QVariant&)), this, SLOT(watchBoxAppeared(const QVariant&)));

  currentWord = characterWords->pickRandomOne(timing);

  connect(this, SIGNAL(boxAppeared()), this, SLOT(startSpeak()));
}

void CharacterPanel::collapseBox()
{
  qDebug() << "collapse";

  textArea->setText("");

  boxAnim->setDuration(600);
  boxAnim->setEasingCurve(QEasingCurve::InOutQuart);
  boxAnim->setStartValue(textArea->geometry());
  boxAnim->setEndValue(QRect(0, 0, 0, 0));

  boxAnim->start();
}

void CharacterPanel::watchBoxAppeared(const QVariant& animVal)
{
  if(animVal == boxAnim->endValue()){
    disconnect(boxAnim, SIGNAL(valueChanged(const QVariant&)), this, SLOT(watchBoxAppeared(const QVariant&)));
    emit boxAppeared();
  }
}

void CharacterPanel::startSpeak()
{
  QString escapedStr = currentWord->sentence.replace("\\n", "\n");

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
    // end this process itself
    speakTimer->stop();

    emit speakEnd();
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
    replyBox->setParent(nullptr);

    boxTimer->start();
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
