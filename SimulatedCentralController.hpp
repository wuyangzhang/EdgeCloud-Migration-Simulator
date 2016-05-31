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
    void runMarkovDecision();
    std::vector<SimulatedEdgeCloud*>* _cloudList;
    std::vector<SimulatedClient*>* _clientList;
    
    int checkOptimalConnectedServer(int clientAddr, int cloudAddr);
    int checkLowestLoadServer(int clientAddr);
    
    void printConnectedClient();
private:
    
    MarkovProcess* _mdp;
    ValueIteration* _vi;
    ReactiveQoS* _reQos;
    
};

#endif /* SimulatedCentralController_hpp */
