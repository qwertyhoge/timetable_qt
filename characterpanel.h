#ifndef CHARACTERPANEL_H
#define CHARACTERPANEL_H

#include <QDockWidget>
#include <QTextEdit>

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

  CharacterWords *characterWords;
  WordTree *currentWord;
  QTimer *speakTimer;
  int showMessageMSec = 2000;

  void speakWord(CharacterWords::Timings timing);
  void setConvo();

protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void sendMenuOpen();
  void collapseMessage();

public slots:
  void showRunMessage();
  void showMenuOpenMessage();
  void showMenuQuitMessage();
  void showPlanEditStartMessage();
  void showPlanEditCancelMessage();
  void showPlanEditDoneMessage();
  void showPlanCreateMessage();
  void showPlanDeleteMessage();
  void yesReply();
  void noReply();

signals:
  void characterClicked();
};

#endif // CHARACTERPANEL_H
