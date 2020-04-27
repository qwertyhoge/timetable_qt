#ifndef CHARACTERPANEL_H
#define CHARACTERPANEL_H

#include <QDockWidget>
#include <QTextEdit>

class CharacterView;
class CharacterWords;

class CharacterPanel : public QDockWidget
{
  Q_OBJECT

public:
  CharacterPanel(QWidget *parent = nullptr);

private:
  QTextEdit *textArea;
  CharacterView *characterArea;
  CharacterWords *characterWords;

private slots:
  void showMenuMessage();

signals:
  void characterClicked();
};

#endif // CHARACTERPANEL_H
