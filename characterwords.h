#ifndef CHARACTERWORDS_H
#define CHARACTERWORDS_H

#include <QString>
#include <QMap>
#include <QHash>

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
  QString pickRandomOne(Timings timing);

  QString wordsPath = "./characters/proto/words.json";

private:
  QHash<Timings, QString> timingStrings;

  QHash<QString, QVector<QString> > wordList;

};

#endif // CHARACTERWORDS_H
