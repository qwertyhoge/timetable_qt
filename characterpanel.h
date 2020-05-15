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

  CharacterWords *characterWords;
  WordTree *currentWord;

  void speakWord(CharacterWords::Timings timing);
  void setConvo();

private slots:
  void sendMenuOpen();

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
