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
    clientList_timeSlot = new std::vector<int>();

    _mdp = mdp;
    _vi = new ValueIteration(mdp);
    
    _reQos = new ReactiveQoS();
    _reQos->_thredshold_QoS = 160;
    _reQos->_baseResponse = mdp->baseResponse;
    _reQos->_singleHopCost = mdp->singleHopCost;
    _reQos->_totalEdgeCloudNumber = _mdp->totalNumberOfEdgeCloud;
    _reQos->_pattern.left = _mdp->probabilityLeft;
    _reQos->_pattern.stay = _mdp->probabilityStay;
    _reQos->_pattern.right = _mdp->probabilityRight;
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
    delete clientList_timeSlot;
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

MarkovProcess*
SimulatedCentralController::getMDP(){
    return _mdp;
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
SimulatedCentralController::updateMDP_TransitionProbability(vector<double>* pattern){
    _mdp->setTransitionProbability(pattern->at(0), pattern->at(1), pattern->at(2));
}

void
SimulatedCentralController::runMarkovDecision(){
    _vi->pattern = 0; // pattern 0: optimal migration, pattern 1: no migrate
    _vi->solve();
}

double
SimulatedCentralController::predictQoS(int clientAddr, int cloudAddr){
    return _reQos->calculateWeightQoS(clientAddr, cloudAddr, _cloudList->at(cloudAddr)->totalClientNumber());
}


vector<int>*
SimulatedCentralController::checkClient_timeSlot(int timeSlot){
    clientList_timeSlot->clear();
    for(int i = 0; i < clientList()->size(); i++){
        if(std::find(clientList()->at(i)->_timeSlot->begin(), clientList()->at(i)->_timeSlot->end(), timeSlot) != clientList()->at(i)->_timeSlot->end()){
            clientList_timeSlot->push_back(i);
        }
    }
    return clientList_timeSlot;
}

int
SimulatedCentralController::checkOptimalConnectedServer(int clientAddr, int cloudAddr){
    
    if(_reQos->exceedQoSThreshold(clientAddr, cloudAddr, _cloudList->at(cloudAddr)->totalClientNumber()+_cloudList->at(cloudAddr)->reservedClientNumber())){
        int targetCloud = _mdp->getAction(cloudAddr, clientAddr);
        cloudList()->at(targetCloud)->reservedClientNumber(cloudList()->at(targetCloud)->reservedClientNumber() + 1);
        cloudList()->at(targetCloud)->serviceCount++;
        return  targetCloud;// launch Markov Decision Model
    }else{
        cloudList()->at(cloudAddr)->reservedClientNumber(cloudList()->at(cloudAddr)->reservedClientNumber() + 1);

        cloudList()->at(cloudAddr)->addPotentialClientNumber();
        cloudList()->at(cloudAddr)->serviceCount++;
        return cloudAddr; // not migrate!
    }
}

int
SimulatedCentralController::checkLowestLoadServer(int clientID){
    struct{
        bool operator()(SimulatedEdgeCloud* a, SimulatedEdgeCloud*b){
            return a->totalClientNumber() + a->reservedClientNumber() < b->totalClientNumber() + b->reservedClientNumber();
        }
    } customLess;
    
    
    SimulatedEdgeCloud* cloud = *std::min_element(_cloudList->begin(), _cloudList->end(), customLess);
    cloudList()->at(cloud->myAddr())->reservedClientNumber(cloudList()->at(cloud->myAddr())->reservedClientNumber() + 1);
    cloudList()->at(cloud->myAddr())->serviceCount++;
    return cloud->myAddr();
}

int
SimulatedCentralController::checkNerverMigrateServer(int cloudAddr){
    cloudList()->at(cloudAddr)->addPotentialClientNumber();
    cloudList()->at(cloudAddr)->serviceCount++;
    return cloudAddr;
}

int
SimulatedCentralController::checkNearestServer(int clientLocation){
    int server = clientLocation ;
    cloudList()->at(server)->addPotentialClientNumber();
    cloudList()->at(server)->serviceCount++;
    return server;
}

void
SimulatedCentralController::printConnectedClient(){
    for(auto i = 0; i < cloudList()->size(); i++){
        printf("[CentralController] cloud %d connects %d clients\n", i, cloudList()->at(i)->totalClientNumber());
    }
    printf("\n");
}

void
SimulatedCentralController::computeAverageResponseTime(){
    
    double total = 0;
    for(auto i = 0; i < timeSlotResponseTime.size(); ++i){
        total+=timeSlotResponseTime.at(i);
    }
    
    printf("Average Response time is %f\n", total/timeSlotResponseTime.size());
}

void
SimulatedCentralController::computeAveragePredictTime(){
    double total = 0;
    for(auto i = 0; i < timeSlotResponseTime.size(); ++i){
         total+=timeSlotResponseTime.at(i);
    }
    
    printf("\n");
}

void
SimulatedCentralController::addTimeSlotResponseTime(double time){
    timeSlotResponseTime.push_back(time);
}

void
SimulatedCentralController::printAverageResponseTime(){
    for(auto i = 0; i < clientAverageResponseTime.size(); ++i){
        printf("[CentralController] move %d average response time %f\n", i, clientAverageResponseTime.at(i));
    }
    printf("\n");

}

void
SimulatedCentralController::printAveragePredictTime(){
    for(auto i = 0; i < clientPredictResponseTime.size(); ++i){
        printf("[CentralController] move %d average predict response time %f\n", i, clientPredictResponseTime.at(i));
    }
    printf("\n");
}

void
SimulatedCentralController::printTimeSlotResponseTime(){
    for(auto i = 0; i < timeSlotResponseTime.size(); ++i){
        printf("[CentralController] slot %d average response time %f\n", i, timeSlotResponseTime.at(i));
    }
    printf("\n");
}

void
SimulatedCentralController::printServiceCount(){
    for(auto i = 0; i < cloudList()->size(); ++i){
        //printf("CentralController cloud %d service time, %d\n",i, cloudList()->at(i)->serviceCount++);
        printf("CentralController cloud service time, %d.0000\n", cloudList()->at(i)->serviceCount++);
    }
    printf("\n");

}
