//
//  reactiveQoS.cpp
//  MarkovProcess
//
//  Created by Wuyang on 5/30/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "reactiveQoS.hpp"

ReactiveQoS::ReactiveQoS(){
    //this->clientLocation = 0;
    //this->edgeCloudLocation = 0;
    _baseResponse = 120;
    _singleHopCost = 10;
    _totalEdgeCloudNumber = 0;
    this->_thredshold_QoS = 0;
}

ReactiveQoS::~ReactiveQoS(){
    
}

double
ReactiveQoS::calculateQoS(int clientLocation, int edgeCloudLocation, int connectClientNum){
    int clientCloudDistance = std::abs(clientLocation - edgeCloudLocation);
    clientCloudDistance =  std::min(clientCloudDistance, _totalEdgeCloudNumber - clientCloudDistance);
    
    double workloadCost = 0;
    if(connectClientNum < 10){
        workloadCost = connectClientNum * 1;
    }else if(connectClientNum < 12){
        workloadCost = connectClientNum * 3.5;
    }else if(connectClientNum < 15){
        workloadCost = connectClientNum * 14.5;
    }else if(connectClientNum < 25){
        workloadCost = connectClientNum * 38.5;
    }else{
        workloadCost = connectClientNum * 100;
    }
    
    return _baseResponse + clientCloudDistance * _singleHopCost + workloadCost;
}

bool
ReactiveQoS::exceedQoSThreshold(int clientLocation, int edgeCloudLocation, int connectClientNum){
    if(this->calculateQoS(clientLocation, edgeCloudLocation, connectClientNum) > _thredshold_QoS){
        return true;
    }else
        return false;
}