#include "replybox.h"

#include <QHBoxLayout>

ReplyBox::ReplyBox(QWidget *parent) : QFrame(parent)
{
  QHBoxLayout *layout = new QHBoxLayout();
  setLayout(layout);

  yesButton = new QPushButton("Yes");
  connect(yesButton, SIGNAL(clicked()), this, SIGNAL(yesClicked()));
  noButton = new QPushButton("No");
  connect(noButton, SIGNAL(clicked()), this, SIGNAL(noClicked()));

  layout->addWidget(yesButton);
  layout->addWidget(noButton);

  QPalette plt = palette();
  plt.setColor(QPalette::Background, QColor(255, 255, 255));
  setPalette(plt);
  setAutoFillBackground(true);

  setFrameShape(QFrame::Box);
  setFrameShadow(QFrame::Sunken);
}

void ReplyBox::processYesClicked()
{
  emit yesClicked();
}

void ReplyBox::processNoClicked()
{
  emit noClicked();
}
