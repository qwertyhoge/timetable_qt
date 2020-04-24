#ifndef CHARACTERWORDS_H
#define CHARACTERWORDS_H

#include <QString>
#include <QMap>

class CharacterWords
{
public:
  CharacterWords();

  bool loadWords();
  bool parseWordsJson(QByteArray json);

  QString wordsPath = "./characters/proto/words.json";
  QStringList timings;
  QMap<QString, QVector<QString> > wordList;
};

#endif // CHARACTERWORDS_H
