#ifndef WaterObjects_hydraulicsolver_h
#define WaterObjects_hydraulicsolver_h

#include "network.h"

namespace WaterObjects {
  
  class HydraulicSolver {
  public:
    void setNetwork(Network& network);
    Network& network();
    
    time_t stepSize();
    void setStepSize(time_t seconds);
    
    void solveSteadyState();
    time_t timeToNextStep();
    void nextStep();
    
    int iterations();
    double relativeError();
    
  private:
    
    
  };
  
}


#endif
