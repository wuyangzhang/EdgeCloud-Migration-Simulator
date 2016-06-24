//
//  SimulatedEdgeCloud.cpp
//  MarkovProcess
//
//  Created by Wuyang on 5/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include <ctime>
#include <fstream>

#include "SimulatedEdgeCloud.hpp"
#include "SimulatedCentralController.hpp"


SimulatedEdgeCloud::SimulatedEdgeCloud(const int addr){
    _myAddr = addr;
   
    totalClientNumber(4);
    _potentialClient = 0;
    _controller = new SimulatedCentralController();
    
    //define cost
    _baseResponse = 120;
    _singleHopCost = 8;
    _unitLoadCost = 100;
    
    //client
    _potentialClient = 0;
}

void
SimulatedEdgeCloud::init(SimulatedCentralController* const control, bool const readLoad){
    if(!readLoad){
        generateBaseWorkload();
        writeBaseWorkload();
    }else{
        readBaseWorkload();
    }
    appWorkload(0);
    
    //debug use
    baseWorkload(0.2);
    totalWorkload(baseWorkload());
    
    _controller = control;
    _totalEdgeCloudNumber = _controller->cloudList()->size();
}

SimulatedEdgeCloud::~SimulatedEdgeCloud(){
    
}

void
SimulatedEdgeCloud::myAddr(const int addr){
    _myAddr = addr;
}

int
SimulatedEdgeCloud::myAddr() const{
    return _myAddr;
}

void
SimulatedEdgeCloud::totalWorkload(const double load){
    _totalWorkload = load;
}

double
SimulatedEdgeCloud::totalWorkload() const{
    return _totalWorkload;
}

void
SimulatedEdgeCloud::baseWorkload(const double load){
    _baseWorkload = load;
}

double
SimulatedEdgeCloud::baseWorkload()const {
    return _baseWorkload;
}

void
SimulatedEdgeCloud::writeBaseWorkload(){
    
    char fileName[100];
    std::sprintf(fileName, "./server/loadSample_%d", _myAddr);
    
    std::ofstream out;
    out.open (fileName);
    out << baseWorkload()<<"\n";
    out.close();
    
}

void
SimulatedEdgeCloud::readBaseWorkload(){
    
    char fileName[100];
    std::sprintf(fileName, "./server/loadSample_%d", _myAddr);
    std::ifstream in;
    in.open(fileName);
    
    std::string line;
    
    if (in.is_open())
    {
        while ( getline (in,line) )
        {
            baseWorkload(atof(line.c_str()));
        }
        in.close();
    }
    
}

void
SimulatedEdgeCloud::appWorkload(const double load){
    _appWorkload = load;
}

double
SimulatedEdgeCloud::appWorkload() const{
    return _appWorkload;
}

void
SimulatedEdgeCloud::totalClientNumber(const int num){
    _totalClientNumber = num;
}

int
SimulatedEdgeCloud::totalClientNumber() const{
    return _totalClientNumber;
}

int
SimulatedEdgeCloud::reservedClientNumber()const{
    return _potentialClient;
}

void
SimulatedEdgeCloud::reservedClientNumber(int num){
    _potentialClient = num;
}

void
SimulatedEdgeCloud::addPotentialClientNumber(){
    _potentialClient++;
}

void
SimulatedEdgeCloud::substractPotentialClientNumber(int num){
    _potentialClient -= num;
}

void
SimulatedEdgeCloud::printCloudState(){
    printf("[SimulatedEdgeCloud %d] workload %f, connected client: \n", _myAddr, totalWorkload());
    for(std::vector<int>::iterator it = clientAddr.begin(); it!= clientAddr.end(); ++it){
        printf("%d\n", (*it));
    }
}
void
SimulatedEdgeCloud::generateBaseWorkload(){
    // baseload ( 0, 0.2)
    srand(_myAddr);
    double load = rand() / double(RAND_MAX) / 5;
    baseWorkload(load);
}

void
SimulatedEdgeCloud::getConnected(int cliAddr){
    //add client to client list, total client add one, reserverd client minus one, report workload
    clientAddr.push_back(cliAddr);
    totalClientNumber(totalClientNumber() + 1);
    reservedClientNumber(reservedClientNumber() - 1);
}

void
SimulatedEdgeCloud::disconnect(int cliAddr){
    // remove this client from client list, total client minus one
    clientAddr.erase(std::remove(clientAddr.begin(), clientAddr.end(),cliAddr), clientAddr.end());
    totalClientNumber(totalClientNumber() - 1);
}

void
SimulatedEdgeCloud::updateWorkload(){
    double appLoad = totalClientNumber() * _clientLoadWeight;
    appWorkload(appLoad);
    totalWorkload(baseWorkload()+appWorkload());
}

void
SimulatedEdgeCloud::reportWorkload(){
    _controller->updateWorkload(_myAddr, totalWorkload());
}

void
SimulatedEdgeCloud::reportConnectedClient(){
    _controller->updateConnectedClient(_myAddr, totalClientNumber() + reservedClientNumber());
}

double
SimulatedEdgeCloud::computeResponseTime(int clientPosition){
    /* calculate cost */
    int clientCloudDistance = std::abs(clientPosition - _myAddr);
    clientCloudDistance =  min(clientCloudDistance, std::abs(_totalEdgeCloudNumber - clientCloudDistance));
    
    double workloadCost = 0;
    if(totalClientNumber() < 10){
        workloadCost = totalClientNumber() * 1;
    }else if(totalClientNumber() < 12){
        workloadCost = totalClientNumber() * 3.5;
    }else if(totalClientNumber() < 15){
        workloadCost = totalClientNumber() * 14.5;
    }else if(totalClientNumber() < 25){
        workloadCost = totalClientNumber() * 38.5;
    }else{
        workloadCost = totalClientNumber() * 100;
    }
    //printf("clientCloudDistance %d, totalClientNumber %d, workloadCost %f\n", clientCloudDistance, totalClientNumber(), workloadCost);
    return _baseResponse + clientCloudDistance * _singleHopCost + workloadCost;
}
