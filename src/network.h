#ifndef __WaterObjects__network__
#define __WaterObjects__network__


#define SEQ_t std::list


#include <iostream>
#include <list>
#include <deque>

#include <boost/shared_ptr.hpp>
#define spr(type) typedef boost::shared_ptr<type> ptr

namespace WaterObjects {
  
  typedef enum {
    junction, tank, reservoir, pipe, pump, valve
  } element_t;
  
#pragma mark Base Type
  class WaterObject {
  public:
    spr(WaterObject);
    const std::string& name();
    void setName(const std::string& name);
    const std::string& description();
    void setDescription(const std::string& desc);
  protected:
    WaterObject() {};
    element_t _type;
  private:
    std::string _name, _description;
  };
  

  
  
  
#pragma mark - Node Elements
  class Pipe; // forward declaration
  class Junction : public WaterObject {
  public:
    spr(Junction);
    Junction() { _type = junction; };
    
    std::pair<double,double> location();
    void setLocation(double lat, double lon);
    
    void setElevation(double elevation);
    double elevation();
    
    void setBaseDemand(double demand);
    double baseDemand();
        
  private:
    double _longitude, _latitude, _elevation;
    double _head, _demand, _baseDemand;
    SEQ_t<Pipe*> _pipes;
  };
  
  class Tank : public Junction {
  public:
    spr(Tank);
    Tank() { _type = tank; };
  };
  class Reservoir : public Tank {
    spr(Reservoir);
    Reservoir() { _type = reservoir; };
  };
  
  
  
  
#pragma mark - Link Elements
  
  class Pipe : public WaterObject {
  public:
    spr(Pipe);
    Pipe(Junction::ptr start, Junction::ptr end);
    Junction* start();
    Junction* end();
    void setStartJunction(Junction::ptr start);
    void setEndJunction(Junction::ptr end);
    void reverse();
    
  private:
    Junction::ptr _start, _end;
    double _length, _diameter, _roughness, _flow;
  };
  
  class Pump : public Pipe {
  };
  class Valve : public Pipe {
  };
  
  
  
  
#pragma mark - Network
  
  class Network : public WaterObject {
    SEQ_t<Junction*> junctions;
    SEQ_t<Pipe*> pipes;
  };
  
  
}

#endif /* defined(__WaterObjects__network__) */
