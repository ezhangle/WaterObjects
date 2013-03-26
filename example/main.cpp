//
//  main.cpp
//  testApp
//
//  Created by Sam Hatchett on 3/20/13.
//  Copyright (c) 2013 OpenWaterAnalytics. All rights reserved.
//

#include <iostream>

#include "modelfile.h"

using namespace std;
using namespace WaterObjects;

int main(int argc, const char * argv[])
{
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " <model/file.inp>" << endl;
    return 1;
  }
  
  string file(argv[1]);
  
  cout << "opening model file: " << file << endl;
  
  ModelFile model(file);
  
  
  
  return 0;
}

