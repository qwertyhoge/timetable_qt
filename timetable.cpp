#include "timetable.h"
#include "dayframe.h"
#include "dayconsts.h"

#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDesktopServices>
#include <QUrl>

Timetable::Timetable(QWidget *parent)
  : QWidget(parent)
{

  move(0, 0);
  layout = new QVBoxLayout();
  setLayout(layout);
  layout->setSpacing(0);

  for(int day = 0; day < 7; day++){
    DayFrame *dayFrame = new DayFrame(DayConsts::intToDayNums(day));
    layout->addWidget(dayFrame);
    dayFrames[day] = dayFrame;

    connect(dayFrames[day], SIGNAL(labelWidthDefined(const int, DayConsts::DayNums)), this, SLOT(justifyDayFrameLabel(const int, DayConsts::DayNums)));
    connect(dayFrames[day], SIGNAL(planClicked(PlanFrame*)), this, SIGNAL(planClicked(PlanFrame*)));
  }

  switchHighlightedDay(DayConsts::intToDayNums(QDate::currentDate().dayOfWeek() % 7));
}

void Timetable::setPlan(Plan *newPlan)
{
  dayFrames[newPlan->getDayNum()]->addPlan(newPlan);

  if(newPlan->getDayNum() == QDate::currentDate().dayOfWeek() % 7){
    PlanTime startTime = newPlan->getStartTime();
    PlanTime endTime = newPlan->getEndTime();
    PlanTime prelimTime = startTime - PlanTime(0, 5);

    registeredPlans[startTime.hour][startTime.minute].registerPlan(START_BELL, newPlan);
    registeredPlans[endTime.hour][endTime.minute].registerPlan(END_BELL, newPlan);

    registeredPlans[prelimTime.hour][prelimTime.minute].registerPlan(PRELIM_BELL, newPlan);
  }
}

void Timetable::justifyDayFrameLabel(const int labelWidth, DayConsts::DayNums dayNum)
{
  qDebug() << labelWidth;
  auto searchMaxWidth = [](int labelWidth[7]){
    int max = -1;
    for(int i = 0; i < 7; i++){
      if(labelWidth[i] == -1){
        return -1;
      }
      if(max < labelWidth[i]){
        max = labelWidth[i];
      }
    }
    return max;
  };

  dayFrameLabelWidths[dayNum] = labelWidth;
  int maxWidth = searchMaxWidth(dayFrameLabelWidths);
  if(maxWidth != -1){
    for(int i = 0; i < 7; i++){
      dayFrames[i]->setLabelWidth(maxWidth);
    }
  }

}

void Timetable::switchHighlightedDay(DayConsts::DayNums dayNum)
{
  for(int i = 0; i < 7; i++){
    if(i == dayNum){
      dayFrames[i]->highlight();
    }else{
      dayFrames[i]->lowlight();
    }
  }

  dayFrames[dayNum]->fillReservedPlans(registeredPlans);
}

void Timetable::playBell(const QUrl bellPath)
{
  bellPlayer->setMedia(bellPath);
  bellPlayer->setVolume(40);
  bellPlayer->play();
}

void Timetable::deletePlanFrame(PlanFrame *plan)
{
  dayFrames[plan->getPlanData()->getDayNum()]->deletePlanFrame(plan);
}

bool Timetable::switchPlanRegistration(const Plan *older, const Plan *newer)
{
  PlanTime olderStart = older->getStartTime();
  PlanTime olderPrelim = olderStart - PlanTime(0, 5);
  PlanTime olderEnd = older->getEndTime();
  bool removeSucceed = true;
  removeSucceed &= registeredPlans[olderPrelim.hour][olderPrelim.minute].removeRegistration(PRELIM_BELL, older);
  removeSucceed &= registeredPlans[olderStart.hour][olderStart.minute].removeRegistration(START_BELL, older);
  removeSucceed &= registeredPlans[olderEnd.hour][olderEnd.minute].removeRegistration(END_BELL, older);

  PlanTime newerStart = newer->getStartTime();
  PlanTime newerPrelim = newerStart - PlanTime(0, 5);
  PlanTime newerEnd = newer->getEndTime();
  registeredPlans[newerPrelim.hour][newerPrelim.minute].registerPlan(PRELIM_BELL, newer);
  registeredPlans[newerStart.hour][newerStart.minute].registerPlan(START_BELL, newer);
  registeredPlans[newerEnd.hour][newerEnd.minute].registerPlan(END_BELL, newer);

  return removeSucceed;
}

void Timetable::loadFromJson(const QByteArray json)
{
  QJsonDocument jsonDoc = QJsonDocument::fromJson(json);

  if(!jsonDoc.isNull()){

    for(int day = 0; day < 7; day++){
      dayFrames[day]->clearPlans();
    }
    for(int h = 0; h < 24; h++){
      for(int m = 0; m < 60; m++){
        registeredPlans[h][m].clear();
      }
    }

    for(int day = 0; day < 7; day++){
      QJsonValue dayInfo = jsonDoc[day];
      QJsonArray dayPlans = dayInfo["plans"].toArray();
      for(auto plan = dayPlans.begin(); plan != dayPlans.end(); plan++){
        QJsonObject planObj = (*plan).toObject();
        QString planName = planObj["planName"].toString();
        PlanTime *startTime = PlanTime::parseTime(planObj["startTime"].toString(), ':');
        PlanTime *endTime = PlanTime::parseTime(planObj["endTime"].toString(), ':');
        QJsonArray dirArr = planObj["workingDirs"].toArray();
        QVector<QDir> workingDirs;
        for(auto dir : dirArr){
          workingDirs.push_back(QDir(dir.toString()));
        }

        Plan *loadedPlan = new Plan(planName, startTime, endTime, DayConsts::intToDayNums(day), workingDirs);
        setPlan(loadedPlan);
      }
    }

  }
}

QJsonArray Timetable::exportAsJson()
{
  QJsonArray jsonTimetable;
  QString dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  for(int i = 0; i < 7; i++){
    QJsonObject dayObj;
    dayObj.insert("day", dayNames[i]);

    dayObj.insert("plans", dayFrames[i]->extractDayJsonArray());

    jsonTimetable.push_back(dayObj);
  }

  return jsonTimetable;
}

void Timetable::processPlanTimings(const QTime &currentTime, DayConsts::DayNums dayNum, bool dayChanged)
{
  if(dayChanged){
    switchHighlightedDay(dayNum);
  }

  RegisteredPlan &reserved = registeredPlans[currentTime.hour()][currentTime.minute()];
  qDebug() << "highestpri: " << reserved.highestBellPri();

  if(reserved.highestBellPri() == START_BELL){
    emit planStartedMessage(CharacterWords::PLAN_START);
    for(auto planRef : reserved.willStartPlans()){
      for(QString path : planRef->dirsAsString().split(';')){
        QDir workingDir = QDir(path);
        if(workingDir.exists()){
          QUrl dirUrl = "file:///" + workingDir.path();
          QDesktopServices::openUrl(dirUrl);
        }
      }
    }
  }else if(reserved.highestBellPri() == END_BELL){
    emit planEndedMessage(CharacterWords::PLAN_END);
  }else if(reserved.highestBellPri() == PRELIM_BELL){
    emit planPrelimMessage(CharacterWords::PLAN_PRELIM);
  }

  bellProperBell(currentTime);
}

void Timetable::bellProperBell(const QTime currentTime)
{
  switch(registeredPlans[currentTime.hour()][currentTime.minute()].highestBellPri()){
    case NONE_BELL:
      break;
    case START_BELL:
      playBell(QUrl("qrc:/sounds/period_bell.mp3"));
      break;
    case END_BELL:
      playBell(QUrl("qrc:/sounds/period_bell.mp3"));
      break;
    case PRELIM_BELL:
      playBell(QUrl("qrc:/sounds/first_bell.mp3"));
      break;
  }
}

void Timetable::addPlan(Plan* newPlan)
{
  setPlan(newPlan);
}
