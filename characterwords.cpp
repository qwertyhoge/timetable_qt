#include "characterwords.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QRandomGenerator>

WordTree::WordTree()
{

}

WordTree::WordTree(QString sentence)
  : sentence(sentence)
{

}

WordTree::WordTree(QString sentence, WordTree *yes, WordTree *no)
  : sentence(sentence), convoYes(yes), convoNo(no)
{

}

bool WordTree::parseConvo(QString ynStr, QString sentence)
{
  if(ynStr.length() == 1){
    if(ynStr[0] == 'y'){
      convoYes = new WordTree(sentence);
      return true;
    }else if(ynStr[0] == 'n'){
      convoNo = new WordTree(sentence);
      return true;
    }else{
      return false;
    }
  }else{
    if(ynStr[0] == 'y'){
      return convoYes->parseConvo(ynStr.mid(1), sentence);
    }else if(ynStr[0] == 'n'){
      return convoNo->parseConvo(ynStr.mid(1), sentence);
    }else{
      return false;
    }
  }
}

bool WordTree::hasConvo()
{
  return convoYes != nullptr && convoNo != nullptr;
}

bool WordTree::allHaveBothConvo()
{
  if(convoYes == nullptr){
    if(convoNo == nullptr){
      return true;
    }else{
      return false;
    }
  }else{
    if(convoNo == nullptr){
      return false;
    }else{
      return convoNo->allHaveBothConvo() && convoYes->allHaveBothConvo();
    }
  }

}

void WordTree::setConvo(WordTree *yes, WordTree *no)
{
  convoYes = yes;
  convoNo = no;
}

void WordTree::deleteFromChildren()
{
  if(convoYes != nullptr){
    convoYes->deleteFromChildren();
  }
  if(convoNo != nullptr){
    convoNo->deleteFromChildren();
  }
  delete this;
}

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

bool CharacterWords::hasTree(Timings timing)
{
  QString timStr = timingStrings[timing];
  return !wordList[timStr].empty();
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
  QHash<QString, QVector<QString> > aliases;
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
        QJsonObject treeObj = word.toObject();
        QString sentence = treeObj["message"].toString();
        WordTree treeRoot = WordTree(sentence);

        QStringList keys = treeObj.keys();
        keys.sort();

        for(auto key : treeObj.keys()){
            treeRoot.parseConvo(key, treeObj[key].toString());
        }
        if(treeRoot.allHaveBothConvo()){
            wordList[timing].push_back(treeRoot);
        }else{
          qDebug() << sentence << " is rejected for children constraint";
        }
      }
    }
  }

  return true;
}

WordTree *CharacterWords::pickRandomOne(Timings timing)
{
  const QString timingKey = timingStrings[timing];
  QVector<WordTree> &wordVec = wordList[timingKey];

  if(wordVec.empty()){
    return nullptr;
  }

  int randomIndex = int(QRandomGenerator::global()->generate() % uint(wordVec.size()));

  return &wordVec[0];
}
