#include "characterwords.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

CharacterWords::CharacterWords()
{
  timings.append("run");
  timings.append("menuopen");
  timings.append("menuquit");
  timings.append("planeditstart");
  timings.append("planeditcancel");
  timings.append("planeditdone");
  timings.append("plancreate");
  timings.append("plandelete");
}

bool CharacterWords::loadWords()
{
  QFile words(wordsPath);

  if(!words.exists()){
    qDebug() << "words file does not exist";
    return false;
  }

  QTextStream inStream(&words);
  QString fileText;
  while(!inStream.atEnd()){
    fileText += inStream.readLine() + '\n';
  }

  return parseWordsJson(fileText.toUtf8());
}


bool CharacterWords::parseWordsJson(QByteArray json)
{
  QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
  if(jsonDoc.isNull()){
    return false;
  }
  QMap<QString, QVector<QString> > aliases;
  QJsonObject aliasPart = jsonDoc["alias"].toObject();
  for(auto timing : timings){
    if(aliasPart.contains(timing)){
      QJsonArray aliasArray = aliasPart[timing].toArray();
      for(auto alias : aliasArray){
        aliases[timing].push_back(alias.toString());
      }
    }
  }

  for(auto timing : timings){
    aliases[timing].push_front(timing);

    for(auto timingAlias : aliases[timing]){
      QJsonArray words = jsonDoc[timingAlias].toArray();
      for(auto word : words){
        aliases[timingAlias].push_back(word.toString());
      }
    }
  }

  return true;
}
