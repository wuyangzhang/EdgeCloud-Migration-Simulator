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
#include <iostream>
#include <string>
#include <fstream>
#include "MarkovProcess.hpp"

class MobilitySimulation{
public:
    MobilitySimulation();
    MobilitySimulation(const int totalNumberOfEdgeCloud, const int totalNumberOfClientsPosition, const int pathLength);
    ~MobilitySimulation();
    
    
    double probabilityLeft;
    double probabilityStay;
    double probabilityRight;
    void setMobilePattern(double,double,double);
    
    int startClientPosition;
    int currentMoveStep;
    int currentClientPosition;
    int totalClientPosition;
    int mobilityPathLength;
    vector<int> mobilityPath;
    void generateRandomMove_line();
    void generateRandomMove_circle();
    void setRandomMove_circle();
    void writeMove();
    void readMove();
    void printMobilityPath();
    

    /* move */
    bool terminateMove;
    void moveToNextClientPosition();
    
    /* edge cloud server*/
    vector<double> generateRandomCloudLoad();
    void writeCloudLoad();
    void readCloudLoad();
    vector<double> cloudLoad;
    void calculateResponseTime_circle();
    void calculateResponseTime_line();
    vector<double> responseTime;
    double calcuateTotalResponseTime();
    void setServiceLocation();
    void setServiceLocation(int);
    /* simulate full movements */
    void simulateMove(int);
    void printCloudLoad();
    MarkovProcess* mdp;

    
private:
    /*server part*/
    int currentEdgeServiceLocation;
    int totalEdgeCloudNumber;
    
    double unitLoadCost;
    double baseResponse;
    double singleHopCost;
    
    double currentLoad;
    double currentReponseTime;
};

#endif /* MobilitySimulation_hpp */
