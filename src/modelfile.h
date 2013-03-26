#ifndef WaterObjects_modelfile_h
#define WaterObjects_modelfile_h

#include <map>
#include <vector>

#include "network.h"
#include "simulation.h"

namespace WaterObjects {
  
  class ModelFile {
    
  public:
    ModelFile(std::string fileUrl);
    
    Simulation& simulation();
    Network& network();
    
    void saveNetwork(Network& network);
    void saveSimulation(Simulation& simulation);
    
  private:
    const std::string _fileStr;
    Network _network;
    Simulation _simulation;
    std::map<std::string, std::string> _junctionPatterns; // <junctionName, patternName>
    
    bool isNewSection(const std::string& line);
    int getSection(const std::string& line);
    
    void parseLineInSection(std::string line, int section);
    void parseJunction(std::string line);
    void parsePipe(std::string line);
    void parseCoordinate(std::string line);
    
    static std::map<int,std::string>sections();
    
    std::map<std::string,Junction::ptr> _junctions;
    std::map<std::string,Pipe::ptr> _pipes;
    
    unsigned int _lineNumber;
    
  };
  
}


#endif
