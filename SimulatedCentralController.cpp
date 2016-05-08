//
//  SimulatedCentralController.cpp
//  MarkovProcess
//
//  Created by Wuyang on 5/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "SimulatedCentralController.hpp"
#include "SimulatedEdgeCloud.hpp"
#include "SimulatedClient.hpp"
SimulatedCentralController::SimulatedCentralController(){
    
}

SimulatedCentralController::SimulatedCentralController(MarkovProcess* mdp){
    _cloudList = new std::vector<SimulatedEdgeCloud*>();
    _clientList = new std::vector<SimulatedClient*>();
    _mdp = mdp;
    _vi = new ValueIteration(mdp);
    
}

SimulatedCentralController::~SimulatedCentralController(){
    delete _mdp;
    delete _vi;
    
    for(auto i = 0; i < clientList()->size(); i++){
        delete clientList()->at(i);
    }
    for(auto i = 0; i < cloudList()->size(); i++){
        delete cloudList()->at(i);
    }
    
    delete _cloudList;
    delete _clientList;
}

std::vector<SimulatedEdgeCloud*>*
SimulatedCentralController::cloudList(){
    return _cloudList;
}

std::vector<SimulatedClient*>*
SimulatedCentralController::clientList(){
    return _clientList;
}

void
SimulatedCentralController::updateWorkload(){
    //report totalWork load of each edge to Makov Model
    for(auto i = 0; i < _cloudList->size(); ++i){
        _mdp->setWorkload(i, _cloudList->at(i)->totalWorkload());
    }
}

void
SimulatedCentralController::updateWorkload(int cloudPostion, double load){
    _mdp->setWorkload(cloudPostion, load);
}

void
SimulatedCentralController::runMarkovDecision(){
    _vi->pattern = 0; // pattern 0: optimal migration, pattern 1: no migrate
    _vi->solve();
}

int
SimulatedCentralController::checkOptimalConnectedServer(int clientAddr, int cloudAddr){
    return _mdp->getAction(cloudAddr, clientAddr);
}
