#include "timetable.h"
#include "dayframe.h"

#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDesktopServices>
#include <QUrl>

ReservedPlan::ReservedPlan()
: bellType(NONE_BELL), planRef(nullptr)
{
}

ReservedPlan::ReservedPlan(BellTypes bell, Plan &plan)
  : bellType(bell), planRef(&plan)
{
}

Timetable::Timetable(QWidget *parent)
  : QWidget(parent)
{
  dayMap["Sunday"] = SUNDAY;
  dayMap["Monday"] = MONDAY;
  dayMap["Tuesday"] = TUESDAY;
  dayMap["Wednesday"] = WEDNESDAY;
  dayMap["Thursday"] = THURSDAY;
  dayMap["Friday"] = FRIDAY;
  dayMap["Saturday"] = SATURDAY;

  move(0, 0);
  layout = new QVBoxLayout();
  setLayout(layout);
  layout->setSpacing(0);

  for(int day = 0; day < 7; day++){
    DayFrame *dayFrame = new DayFrame(nullptr, dayStrings[day]);
    layout->addWidget(dayFrame);
    dayFrames[day] = dayFrame;

    dayFrames[day]->installEventFilter(this);
  }

  switchHighlightedDay(QDate::currentDate().dayOfWeek() % 7);
}

void Timetable::setPlan(Plan *newPlan)
{
  dayFrames[newPlan->dayNum]->addPlan(newPlan);

  connect(newPlan, SIGNAL(planClicked(Plan*)), this, SLOT(raisePlanClicked(Plan*)));

  if(newPlan->dayNum == QDate::currentDate().dayOfWeek() % 7){
    reservedPlans[newPlan->startTime->hour][newPlan->startTime->minute].bellType = START_BELL;
    reservedPlans[newPlan->startTime->hour][newPlan->startTime->minute].planRef = newPlan;
    if(reservedPlans[newPlan->endTime->hour][newPlan->startTime->minute].bellType != START_BELL){
      reservedPlans[newPlan->endTime->hour][newPlan->startTime->minute].bellType = END_BELL;
      reservedPlans[newPlan->endTime->hour][newPlan->startTime->minute].planRef = newPlan;
    }

    PlanTime *prelimTime = *(newPlan->startTime) - new PlanTime(0, 5);
    if(reservedPlans[prelimTime->hour][prelimTime->minute].bellType == NONE_BELL){
      reservedPlans[prelimTime->hour][prelimTime->minute].bellType = PRELIM_BELL;
      reservedPlans[prelimTime->hour][prelimTime->minute].planRef = newPlan;
    }
  }

}

void Timetable::switchHighlightedDay(int dayNum)
{
  for(int i = 0; i < 7; i++){
    if(i == dayNum){
      dayFrames[i]->highlight();
    }else{
      dayFrames[i]->lowlight();
    }
  }

  dayFrames[dayNum]->fillReservedPlans(reservedPlans);
}

bool Timetable::eventFilter(QObject *obj, QEvent *event)
{
  if(!allShown){
    for(int i = 0; i < 7; i++){
      if(obj == dayFrames[i] && event->type() == QEvent::Show){
        labelWidths[i] = dayFrames[i]->labelWidth();

        auto searchMaxWidth = [&]{
          int maxWidth = 0;
          for(int i = 0; i < 7; i++){
            if(labelWidths[i] == -1){
              return -1;
            }else if(labelWidths[i] > maxWidth){
              maxWidth = labelWidths[i];
            }
          }

          return maxWidth;
        };

        int maxWidth = searchMaxWidth();
        if(maxWidth != -1){
          for(int i = 0; i < 7; i++){
            dayFrames[i]->setLabelWidth(maxWidth);
          }
          allShown = true;
        }

        return true;
      }
    }

  }

  return false;
}

void Timetable::playBell(QUrl bellPath)
{
  bellPlayer->setMedia(bellPath);
  bellPlayer->setVolume(40);
  bellPlayer->play();
}

void Timetable::deletePlan(Plan *plan)
{
  dayFrames[plan->dayNum]->deletePlan(plan);
}

void Timetable::loadFromJson(QByteArray json)
{
  QJsonDocument jsonDoc = QJsonDocument::fromJson(json);

  if(!jsonDoc.isNull()){

    for(int day = 0; day < 7; day++){
      dayFrames[day]->clearPlans();
    }
    for(int h = 0; h < 24; h++){
      for(int m = 0; m < 60; m++){
        reservedPlans[h][m].bellType = NONE_BELL;
        reservedPlans[h][m].planRef = nullptr;
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

        Plan *loadedPlan = new Plan(planName, startTime, endTime, day, workingDirs);
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

void Timetable::processPlanTimings(QDateTime &currentDateTime, bool dayChanged)
{
  const QTime &currentTime = currentDateTime.time();

  if(dayChanged){
    switchHighlightedDay(currentDateTime.date().day());
  }

  ReservedPlan &reserved = reservedPlans[currentTime.hour()][currentTime.minute()];

  if(reserved.bellType == START_BELL){
    emit planStartedMessage(CharacterWords::PLAN_START);
    for(QDir &workingDir : reserved.planRef->workingDirs){
      if(workingDir.exists()){
        QUrl dirUrl = "file:///" + workingDir.path();
        QDesktopServices::openUrl(dirUrl);
      }
    }
  }else if(reserved.bellType == END_BELL){
    emit planEndedMessage(CharacterWords::PLAN_END);
  }else if(reserved.bellType == PRELIM_BELL){
    emit planPrelimMessage(CharacterWords::PLAN_PRELIM);
  }

  bellProperBell(currentTime);
}

void Timetable::bellProperBell(QTime currentTime)
{
  switch(reservedPlans[currentTime.hour()][currentTime.minute()].bellType){
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
  qDebug() << "addplan";
  qDebug() << "day: " << newPlan->dayNum;
  setPlan(newPlan);
}

void Timetable::raisePlanClicked(Plan* clickedPlan)
{
  emit planClicked(clickedPlan);
}
