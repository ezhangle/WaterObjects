#include "modelfile.h"
#include "epanetparsers.h"

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/remove_if.hpp>



using namespace WaterObjects;
using namespace std;
using boost::algorithm::trim_copy;




typedef enum {
  title,
  junctions,
  reservoirs,
  tanks,
  pipes,
  pumps,
  valves,
  controls,
  rules,
  demands,
  sources,
  emitters,
  patterns,
  curves,
  quality,
  status,
  roughness,
  energy,
  reactions,
  mixing,
  report,
  times,
  options,
  coordinates,
  vertices,
  labels,
  backdrop,
  tags,
  end,
  unknown
} epanet_section_t;

map<int, string> ModelFile::sections() {
  std::map<int, string> s;
  typedef epanet_section_t st; // section type scoping
  
  s[st::title] = "[TITLE]";
  s[st::junctions] = "[JUNCTIONS]";
  s[st::reservoirs] = "[RESERVOIRS]";
  s[st::tanks] = "[TANKS]";
  s[st::pipes] = "[PIPES]";
  s[st::pumps] = "[PUMPS]";
  s[st::valves] = "[VALVES]";
  s[st::controls] = "[CONTROLS]";
  s[st::rules] = "[RULES]";
  s[st::demands] = "[DEMANDS]";
  s[st::sources] = "[SOURCES]";
  s[st::emitters] = "[EMITTERS]";
  s[st::patterns] = "[PATTERNS]";
  s[st::curves] = "[CURVES]";
  s[st::quality] = "[QUALITY]";
  s[st::status] = "[STATUS]";
  s[st::roughness] = "[ROUGHNESS]";
  s[st::energy] = "[ENERGY]";
  s[st::reactions] = "[REACTIONS]";
  s[st::mixing] = "[MIXING]";
  s[st::report] = "[REPORT]";
  s[st::times] = "[TIMES]";
  s[st::options] = "[OPTIONS]";
  s[st::coordinates] = "[COORDINATES]";
  s[st::vertices] = "[VERTICES]";
  s[st::labels] = "[LABELS]";
  s[st::backdrop] = "[BACKDROP]";
  s[st::tags] = "[TAGS]";
  s[st::end] = "[END]";
  
  return s;
}


ModelFile::ModelFile(std::string fileUrl) : _fileStr(fileUrl) {
  
  // epanet file format
  
  fstream fin;
  fin.open(fileUrl.c_str(), fstream::in); //
  if (!fin.good()) {
    cerr << "could not open file: " << fileUrl << endl;
    return;
  }
  
  string rawLine;
  epanet_section_t section;
  
  _lineNumber = 0;
  while (!fin.eof()) {
    getline(fin, rawLine);
    ++_lineNumber;
    
    string line = boost::algorithm::trim_left_copy(rawLine);
    if (line.find(";") == 0 || line.length() == 0) {
      continue; // nothing to see
    }
    
    if (isNewSection(line)) {
      section = (epanet_section_t)getSection(line);
      cout << "Section :: " << line << " is type: " << section << endl;
    }
    else if (section != epanet_section_t::unknown) {
      parseLineInSection(line, section);
    }
    
    
    
  }
  
  
  // cout << "done parsing" << endl;
  
};


bool ModelFile::isNewSection(const std::string &line) {
  if (line.find("[") != string::npos) {
    return true;
  }
  return false;
}

int ModelFile::getSection(const std::string &line) {
  if (isNewSection(line)) {
    string sectionString(line);
    boost::algorithm::trim(sectionString); // whitespace from both sides
    map<int,string> s = sections();
    BOOST_FOREACH(const int i, s | boost::adaptors::map_keys ) {
      if ( boost::equal(s[i], sectionString) ) {
        return i;
      }
    }
  }
  return epanet_section_t::unknown;
}

void ModelFile::parseLineInSection(string line, int section) {
  // just redirect to the section-specific method.
  typedef epanet_section_t st;
  switch (section) {
    case st::junctions:
      parseJunction(line);
      break;
    case st::pipes:
      parsePipe(line);
      break;
    case st::coordinates:
      parseCoordinate(line);
      break;
    default:
      break;
  }
}


void ModelFile::parseJunction(string line) {  
  junction_parser jp;
  parsedJunction j;
  
  bool parseOk = phrase_parse(line.begin(), line.end(), jp, boost::spirit::ascii::space, j);
  if (parseOk) {
    Junction::ptr newJ( new Junction() );
    newJ->setName(j.id);
    newJ->setBaseDemand(j.demand);
    newJ->setElevation(j.elevation);
    newJ->setDescription(j.description);
    _junctions[j.id] = newJ;
    
    _junctionPatterns[j.id] = j.pattern;
    //cout << "found junction: " << id << " - " << desc << " -- " << elevation << " - " << demand << endl;
  }
  else {
    cerr << "Invalid line (" << _lineNumber << "): " << line << endl;
  }
  
}

void ModelFile::parsePipe(std::string line) {
  pipe_parser pp;
  parsedPipe p;
  Junction::ptr j1, j2;
  
  bool parseOk = phrase_parse(line.begin(), line.end(), pp, boost::spirit::ascii::space, p);
  if (parseOk) {
    if (_junctions.find(p.node1) != _junctions.end()) {
      j1 = _junctions[p.node1];
    }
    if (_junctions.find(p.node2) != _junctions.end()) {
      j2 = _junctions[p.node2];
    }
    
    if (j1 && j2) {
      Pipe::ptr newP( new Pipe(j1,j2) );
      newP->setName(p.id);
      //newP->setDiameter(p.diameter);
      //newP->setLength(p.length);
      _pipes[p.id] = newP;
    }
    
  }
  else {
    cerr << "Invalid line (" << _lineNumber << "): " << line << endl;
  }
}



void ModelFile::parseCoordinate(std::string line) {
  coord_parser cp;
  parsedCoord c;
  Junction::ptr j;
  
  bool parseOk = phrase_parse(line.begin(), line.end(), cp, boost::spirit::ascii::space, c);
  if (parseOk) {
    if (_junctions.find(c.id) != _junctions.end()) {
      j = _junctions[c.id];
      j->setLocation(c.x, c.y);
    }
    else {
      cerr << "Warning: Junction \"" << c.id << "\" not found. Coordinate line " << _lineNumber << endl;
    }
  }
  
}




Network& ModelFile::network() {
  
}