#include "characterwords.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

CharacterWords::CharacterWords()
{

  timingStrings[RUN] = "run";
  timingStrings[MENU_OPEN] = "menuopen";
  timingStrings[MENU_QUIT] = "menuquit";
  timingStrings[PLAN_EDIT_START] = "planeditstart";
  timingStrings[PLAN_EDIT_CANCEL] = "planeditcancel";
  timingStrings[PLAN_EDIT_DONE] = "planeditdone";
  timingStrings[PLAN_CREATE] = "plancreate";
  timingStrings[PLAN_DELETE] = "plandelete";
}

bool CharacterWords::loadWords()
{
  QFile words(wordsPath);

  if(!words.exists()){
    qDebug() << "words file does not exist";
    return false;
  }
  if(!words.open(QIODevice::ReadOnly)){
    qDebug() << "failed to open words file";
    return false;
  }

  QTextStream inStream(&words);
  QString fileText;
  while(!inStream.atEnd()){
    QString line = inStream.readLine();
    line.replace("\\", "\\\\");
    fileText += line + '\n';
  }

  words.close();

  return parseWordsJson(fileText.toUtf8());
}


bool CharacterWords::parseWordsJson(QByteArray json)
{
  QJsonParseError error;
  QJsonDocument jsonDoc = QJsonDocument::fromJson(json, &error);
  if(jsonDoc.isNull()){
    qDebug() << "document is null";
    qDebug() << error.errorString() << " " << error.offset;
    return false;
  }
  QMap<QString, QVector<QString> > aliases;
  QJsonObject aliasPart = jsonDoc["alias"].toObject();
  for(auto timing : timingStrings){
    if(aliasPart.contains(timing)){
      QJsonArray aliasArray = aliasPart[timing].toArray();
      for(auto alias : aliasArray){
        aliases[timing].push_back(alias.toString());
      }
    }
  }

  for(auto timing : timingStrings){
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

QString CharacterWords::pickRandomOne(Timings timing)
{

}
