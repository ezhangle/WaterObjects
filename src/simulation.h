#ifndef WaterObjects_simulation_h
#define WaterObjects_simulation_h

#include "network.h"
#include "hydraulicsolver.h"

namespace WaterObjects {
  
  class Simulation {
  public:
    void run();
    
  private:
    Network _network;
    HydraulicSolver _hydSolver;
    time_t _startTime, _duration;
  };
  
}

#endif
