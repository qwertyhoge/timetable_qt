#ifndef CHARACTERWORDS_H
#define CHARACTERWORDS_H

#include <QString>
#include <QHash>

class WordTree
{
public:
  WordTree();
  WordTree(QString sentence);
  WordTree(QString sentence, WordTree *yes, WordTree *no);
  ~WordTree();

  QString sentence;
  WordTree* convoYes = nullptr;
  WordTree* convoNo = nullptr;

  void setConvo(WordTree *yes, WordTree *no);
  bool hasConvo() const;
  bool allHaveBothConvo() const;
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
    PLAN_DELETE,
    PLAN_START,
    PLAN_END,
    PLAN_PRELIM
  };

  bool loadWords();
  bool hasTree(const Timings timing) const;
  WordTree *pickRandomOne(const Timings timing);

private:
  void destroyTree();
  bool parseWordsJson(const QByteArray json);
  QHash<Timings, QString> timingStrings;
  QHash<QString, QVector<WordTree*> > wordList;

  QString wordsPath = "./characters/proto/words.json";
};

#endif // CHARACTERWORDS_H
