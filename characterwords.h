#ifndef CHARACTERWORDS_H
#define CHARACTERWORDS_H

#include <QString>
#include <QHash>

class WordTree
{
public:
  WordTree(QString sentence);
  WordTree(QString sentence, WordTree *yes, WordTree *no);

  QString sentence;

  WordTree* convoYes = nullptr;
  WordTree* convoNo = nullptr;

  void setConvo(WordTree *yes, WordTree *no);
  bool parseConvo(QString ynStr, QString sentence);

private:
  void deleteFromChildren();
};

class CharacterWords
{
public:
  CharacterWords();

  enum Timings{
    RUN,
    MENU_OPEN,
    MENU_QUIT,
    PLAN_EDIT_START,
    PLAN_EDIT_CANCEL,
    PLAN_EDIT_DONE,
    PLAN_CREATE,
    PLAN_DELETE
  };

  bool loadWords();
  bool parseWordsJson(QByteArray json);
  WordTree *pickRandomOne(Timings timing);

  QString wordsPath = "./characters/proto/words.json";

private:
  QHash<Timings, QString> timingStrings;

  QHash<QString, QVector<WordTree> > wordList;

};

#endif // CHARACTERWORDS_H
