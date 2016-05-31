//
//  SimulatedCentralController.cpp
//  MarkovProcess
//
//  Created by Wuyang on 5/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//
#include <algorithm>
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
    
    _reQos = new ReactiveQoS();
    _reQos->_thredshold_QoS = 150;
    _reQos->_totalEdgeCloudNumber = _mdp->totalNumberOfEdgeCloud;
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
    delete _reQos;
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
SimulatedCentralController::updateConnectedClient(int cloudPosition, int connectedClient){
    _mdp->setConnectedClient(cloudPosition, connectedClient);
}

void
SimulatedCentralController::updateConnectedClient(){
    
}

void
SimulatedCentralController::runMarkovDecision(){
    _vi->pattern = 0; // pattern 0: optimal migration, pattern 1: no migrate
    _vi->solve();
}

int
SimulatedCentralController::checkOptimalConnectedServer(int clientAddr, int cloudAddr){
    //todo add reactive function.
    if(_reQos->exceedQoSThreshold(clientAddr, cloudAddr, _cloudList->at(cloudAddr)->totalClientNumber())){
        return _mdp->getAction(cloudAddr, clientAddr); // launch Markov Decision Model
    }else{
        return cloudAddr; // not migrate!
    }
}

int
SimulatedCentralController::checkLowestLoadServer(int clientAddr){
    struct{
        bool operator()(SimulatedEdgeCloud* a, SimulatedEdgeCloud*b){
            return a->totalClientNumber() < b->totalClientNumber();
        }
    } customLess;
    
    SimulatedEdgeCloud* cloud = *std::min_element(_cloudList->begin(), _cloudList->end(), customLess);
    return cloud->myAddr();
}

void
SimulatedCentralController::printConnectedClient(){
    for(auto i = 0; i < cloudList()->size(); i++){
        printf("[CentralController] cloud %d connects %d clients", i, cloudList()->at(i)->totalClientNumber());
    }
    printf("\n");
}

