#include "timetable.h"
#include "dayframe.h"

#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Timetable::Timetable(QWidget *parent)
  : QWidget(parent)
{
  dayMap["Sunday"] = SUNDAY;
  dayMap["Monday"] = MONDAY;
  dayMap["Tuesday"] = TUESDAY;
  dayMap["Wednesday"] = WEDNESDAY;
  dayMap["Thirsday"] = THIRSDAY;
  dayMap["Friday"] = FRIDAY;
  dayMap["Saturday"] = SATURDAY;

  for(int i = 0; i < 24; i++){
    for(int j = 0; j < 60; j++){
      timeToAlerm[i][j] = NONE_BELL;
    }
  }

  move(0, 0);
  layout = new QVBoxLayout();
  setLayout(layout);
  layout->setSpacing(0);

  for(int day = 0; day < 7; day++){
    DayFrame *dayFrame = new DayFrame(nullptr, dayStrings[day]);
    layout->addWidget(dayFrame);
    dayFrames[day] = dayFrame;
  }

  for(int i = 0; i < 7; i++){
    dayFrames[i]->installEventFilter(this);
    dayFrames[i]->dayLabel->setAutoFillBackground(true);
  }

  highlightCurrentDay(QDate::currentDate().dayOfWeek() % 7);
}


void Timetable::setPlan(Plan *newPlan)
{
  QWidget *parentArea = dayFrames[newPlan->dayNum]->planArea;
  timetableData[newPlan->dayNum].push_back(newPlan);
  newPlan->fitGeometry(parentArea->size());
  newPlan->setParent(parentArea);

  connect(newPlan, SIGNAL(planClicked(Plan*)), this, SLOT(propagatePlanClicked(Plan*)));

  newPlan->show();

  timeToAlerm[newPlan->startTime->hour][newPlan->startTime->minute] = START_BELL;
  if(timeToAlerm[newPlan->endTime->hour][newPlan->startTime->minute] != START_BELL){
    timeToAlerm[newPlan->endTime->hour][newPlan->startTime->minute] = END_BELL;
  }
  PlanTime *prelimTime = *(newPlan->startTime) - new PlanTime(0, 5);
  if(timeToAlerm[prelimTime->hour][prelimTime->minute] == NONE_BELL){
    timeToAlerm[prelimTime->hour][prelimTime->minute] = PRELIM_BELL;
  }

}

void Timetable::highlightCurrentDay(int dayNum)
{
  for(int i = 0; i < 7; i++){
    QPalette plt = palette();

    plt.setColor(QPalette::Background, QColor(248, 251, 254));
    if(i == dayNum){
      plt.setColor(QPalette::Background, QColor(253, 217, 192));
    }

    dayFrames[i]->dayLabel->setPalette(plt);
  }
}

bool Timetable::eventFilter(QObject *obj, QEvent *event)
{
  for(int i = 0; i < 7; i++){
    if(obj == dayFrames[i] && event->type() == QEvent::Resize){
      for(auto plan : timetableData[i]){
        plan->updatePlanGeometry();
      }
      return true;
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
  qDebug() << "remove";

  if(!timetableData[plan->dayNum].removeOne(plan)){
    qCritical() << "failed to delete from vector";
  }
  plan->setParent(nullptr);
  plan->deleteLater();
}

void Timetable::loadFromJson(QByteArray json)
{
  QJsonDocument jsonDoc = QJsonDocument::fromJson(json);

  if(!jsonDoc.isNull()){
    for(int day = 0; day < 7; day++){
      for(auto plan : timetableData[day]){
        deletePlan(plan);
      }
    }
    for(int h = 0; h < 24; h++){
      for(int m = 0; m < 60; m++){
        timeToAlerm[h][m] = NONE_BELL;
      }
    }
    timetableData->clear();
    for(int day = 0; day < 7; day++){
      QJsonValue dayInfo = jsonDoc[day];
      QJsonArray dayPlans = dayInfo["plans"].toArray();
      for(auto plan = dayPlans.begin(); plan != dayPlans.end(); plan++){
        QJsonObject planObj = (*plan).toObject();
        QString planName = planObj["planName"].toString();
        PlanTime *startTime = PlanTime::parseTime(planObj["startTime"].toString(), ':');
        PlanTime *endTime = PlanTime::parseTime(planObj["endTime"].toString(), ':');
        Plan *loadedPlan = new Plan(planName, startTime, endTime, day);

        setPlan(loadedPlan);
      }
    }

  }
}

QJsonArray Timetable::exportAsJson()
{
  QJsonArray jsonTimetable;
  QString dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thirsday", "Friday", "Saturday"};
  for(int i = 0; i < 7; i++){
    QJsonObject dayObj;
    dayObj.insert("day", dayNames[i]);

    QJsonArray dayPlans;
    for(int j = 0; j < timetableData[i].size(); j++){
      QJsonObject planInfo;
      qDebug() << "capsling " << timetableData[i][j]->planName;

      planInfo.insert("planName", timetableData[i][j]->planName);
      planInfo.insert("startTime", timetableData[i][j]->startTime->toString());
      planInfo.insert("endTime", timetableData[i][j]->endTime->toString());

      dayPlans.push_back(planInfo);
    }
    dayObj.insert("plans", dayPlans);

    jsonTimetable.push_back(dayObj);
  }

  return jsonTimetable;
}

void Timetable::bellProperBell(QTime currentTime)
{
  switch(timeToAlerm[currentTime.hour()][currentTime.minute()]){
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

void Timetable::propagatePlanClicked(Plan* clickedPlan)
{
  emit planClicked(clickedPlan);
}
