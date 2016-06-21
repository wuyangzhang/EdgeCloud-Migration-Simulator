//
//  SimulatedCentralController.hpp
//  MarkovProcess
//
//  Created by Wuyang on 5/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#ifndef SimulatedCentralController_hpp
#define SimulatedCentralController_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

#include "MarkovProcess.hpp"
#include "ValueIteration.hpp"
#include "reactiveQoS.hpp"

class SimulatedEdgeCloud;
class SimulatedClient;

class SimulatedCentralController{
public:
    SimulatedCentralController();
    SimulatedCentralController(MarkovProcess* mdp);
    ~SimulatedCentralController();
    std::vector<SimulatedEdgeCloud*>* cloudList();
    std::vector<SimulatedClient*>* clientList();
    void updateWorkload();
    void updateWorkload(int,double);
    void updateConnectedClient(int, int);
    void updateConnectedClient();
    void updateMDP_TransitionProbability(vector<double>*);
    void runMarkovDecision();
    std::vector<SimulatedEdgeCloud*>* _cloudList;
    std::vector<SimulatedClient*>* _clientList;
    vector<int>* checkClient_timeSlot(int timeSlot);
    std::vector<int>* clientList_timeSlot;
    MarkovProcess* getMDP();
    
    int checkOptimalConnectedServer(int clientAddr, int cloudAddr);
    int checkLowestLoadServer(int clientID);
    int checkNerverMigrateServer(int cloudAddr);
    int checkNearestServer(int clientLocation);
    double predictQoS(int clientAddr, int cloudAddr);
    void printConnectedClient();
    
    /* analyze performance */
    void computeAverageResponseTime();
    void computeAveragePredictTime();
    void printAverageResponseTime();
    void printAveragePredictTime();
    void addTimeSlotResponseTime(double time);
    void printTimeSlotResponseTime();
private:
    
    MarkovProcess* _mdp;
    ValueIteration* _vi;
    ReactiveQoS* _reQos;
    
    std::vector<double> clientAverageResponseTime;
    std::vector<double> clientPredictResponseTime;
    std::vector<double> timeSlotResponseTime;
};

#endif /* SimulatedCentralController_hpp */
