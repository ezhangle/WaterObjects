#include "network.h"
#include <boost/algorithm/string.hpp>

using namespace WaterObjects;
using namespace std;

void WaterObject::setName(const string& name) {
  _name = name;
}
const string& WaterObject::name() {
  return _name;
}

void WaterObject::setDescription(const string& desc) {
  _description = desc;
}
const string& WaterObject::description() {
  return _description;
}


void Junction::setElevation(double elevation) {
  _elevation = elevation;
}
double Junction::elevation() {
  return _elevation;
}

void Junction::setBaseDemand(double demand) {
  _baseDemand = demand;
}
double Junction::baseDemand() {
  return _baseDemand;
}

void Junction::setLocation(double lat, double lon) {
  _longitude = lon;
  _latitude = lat;
}
pair<double,double> Junction::location() {
  return make_pair(_latitude, _longitude);
}



Pipe::Pipe(Junction::ptr start, Junction::ptr end) {
  _start = start;
  _end = end;
}