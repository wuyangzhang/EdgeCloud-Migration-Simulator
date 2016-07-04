//
//  MobilitySimulator.hpp
//  MarkovProcess
//
//  Created by Wuyang on 5/7/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#ifndef MobilitySimulator_hpp
#define MobilitySimulator_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

#include "SimulatedEdgeCloud.hpp"
#include "SimulatedClient.hpp"
#include "SimulatedCentralController.hpp"

class MobilitySimulator{
public:
    MobilitySimulator(int totalCloud, int totalClientNumber, int totalClientPosition);
    ~MobilitySimulator();
    
    void simulate();

    //vector<double> mobilityPattern(double, double, double);
    double _hopCost = 0;
    double _baseResp = 120;

private:
    int _totalClientNumber;
    int _totalClientPosition;
    int _totalCloud;
    int _readModel;
    int _queryModel;
    SimulatedCentralController* _controller;
    
    struct mobilityPattern{
        double left;
        double stay;
        double right;
    } _pattern;
    
   };
#endif /* MobilitySimulator_hpp */
