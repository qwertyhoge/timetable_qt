#ifndef REPLYBOX_H
#define REPLYBOX_H

#include <QFrame>
#include <QPushButton>

class ReplyBox : public QFrame
{
  Q_OBJECT
public:
  explicit ReplyBox(QWidget *parent = nullptr);

private:
  QPushButton *yesButton;
  QPushButton *noButton;

signals:
  void yesClicked();
  void noClicked();

public slots:
  void processYesClicked();
  void processNoClicked();
};

#endif // REPLYBOX_H
