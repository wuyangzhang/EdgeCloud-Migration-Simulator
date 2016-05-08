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
    MobilitySimulator(int totalClientNumber, int totalClientPosition, int totalCloud);
    ~MobilitySimulator();
    
    void simulate();

    //vector<double> mobilityPattern(double, double, double);
private:
    int _totalClientNumber;
    int _totalClientPosition;
    int _totalCloud;
    int _clientModel;
    SimulatedCentralController* _controller;
};
#endif /* MobilitySimulator_hpp */
