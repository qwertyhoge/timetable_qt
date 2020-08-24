#ifndef BELLTYPE_H
#define BELLTYPE_H

enum BellTypes{
  NONE_BELL,
  START_BELL,
  END_BELL,
  PRELIM_BELL
};

class Plan;

class ReservedPlan
{
public:
  ReservedPlan();
  ReservedPlan(BellTypes bell, Plan &plan);

  BellTypes bellType;
  Plan *planRef;
};


#endif // BELLTYPE_H
