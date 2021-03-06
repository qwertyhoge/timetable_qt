#ifndef CHARACTERPANEL_H
#define CHARACTERPANEL_H

#include <QDockWidget>
#include <QTextEdit>
#include <QQueue>
#include <QPropertyAnimation>

#include <characterwords.h>

class CharacterView;
class ReplyBox;

class CharacterPanel : public QDockWidget
{
  Q_OBJECT

public:
  CharacterPanel(QWidget *parent = nullptr);

private:
  QTextEdit *textArea;
  CharacterView *characterArea;
  ReplyBox *replyBox;
  QWidget *convoArea;

  QPropertyAnimation *openAnim;
  QPropertyAnimation *closeAnim;
  QTimer *closeTimer;
  QTimer *speakTimer;
  int showMessageMSec = 2000;
  int charInterval = 70;
  int currentState = HIDDEN;
  enum AnimState{
    HIDDEN,
    OPENING,
    CLOSING,
    OPENED
  };

  CharacterWords *characterWords;
  WordTree *currentWord;
  QQueue<QChar> wordQue;

  void speakWord(const CharacterWords::Timings timing);

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void collapseBox();
  void watchBoxAppeared(const QVariant&);
  void watchBoxDisappeared(const QVariant&);
  void startSpeak();
  void speakCharByChar();
  void setConvo();

public slots:
  void processTimings(const CharacterWords::Timings timing);
  void yesReply();
  void noReply();

signals:
  void characterClicked();
  void speakEnd();
};

#endif // CHARACTERPANEL_H
