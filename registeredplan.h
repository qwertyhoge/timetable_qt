#ifndef REGISTEREDPLAN_H
#define REGISTEREDPLAN_H

#include <QVector>

enum BellTypes{
  NONE_BELL,
  START_BELL,
  END_BELL,
  PRELIM_BELL
};

class Plan;

class RegisteredPlan
{
public:
  QVector<QPair<BellTypes, const Plan*> > refs;

  RegisteredPlan(){
  }

  void registerPlan(BellTypes bellType, const Plan* planRef){
    refs.push_back(QPair<BellTypes, const Plan*>(bellType, planRef));
  }

  bool removeRegistration(BellTypes bellType, const Plan *planRef){
    for(int i = 0;i < refs.length(); i++){
      if(refs[i].first == bellType && refs[i].second == planRef){
        refs.removeAt(i);
        return true;
      }
    }

    return false;
  }

  BellTypes highestBellPri(){
    BellTypes highest = NONE_BELL;
    for(auto ref : refs){
      if(ref.first == START_BELL){
        highest = START_BELL;
      }else if(ref.first == END_BELL && highest != START_BELL){
        highest = END_BELL;
      }else if(ref.first == PRELIM_BELL && highest == NONE_BELL){
        highest = PRELIM_BELL;
      }
    }

    return highest;
  }

  QVector<const Plan*> willStartPlans(){
    QVector<const Plan*> plans;
    for(auto ref : refs){
      if(ref.first == START_BELL){
        plans.push_back(ref.second);
      }
    }

    return plans;
  }

  void clear(){
    refs.clear();
  }


};


#endif // REGISTEREDPLAN_H
