//
//  MobilitySimulation.hpp
//  MarkovProcess
//
//  Created by Wuyang on 5/3/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#ifndef MobilitySimulation_hpp
#define MobilitySimulation_hpp

#include <stdio.h>
#include "MarkovProcess.hpp"

class MobilitySimulation{
public:
    MobilitySimulation();
    MobilitySimulation(struct mobilityProbability);
    MobilitySimulation(double probabilityLeft, double probabilityStay, double probabilityRight);
    ~MobilitySimulation();
    struct mobilityProbability{
        double probabilityLeft;
        double probabilityStay;
        double probabilityRight;
    };
    double probabilityLeft;
    double probabilityStay;
    double probabilityRight;
    
    int startClientPosition;
    int mobilityPathLength;
    vector<int> mobilityPath;
    void generateRandomMove(int,int);
    void generateRandomMove_circle(int,int,int);

    void printMobilityPath();
    
private:
};

#endif /* MobilitySimulation_hpp */
