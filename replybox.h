#ifndef REPLYBOX_H
#define REPLYBOX_H

#include <QFrame>
#include <QPushButton>

class ReplyBox : public QFrame
{
  Q_OBJECT
public:
  explicit ReplyBox(QWidget *parent = nullptr);

  QPushButton *yesButton;
  QPushButton *noButton;

signals:
  void yesClicked();
  void noClicked();

public slots:
};

#endif // REPLYBOX_H
