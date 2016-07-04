//
//  SimulatedEdgeCloud.hpp
//  MarkovProcess
//
//  Created by Wuyang on 5/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#ifndef SimulatedEdgeCloud_hpp
#define SimulatedEdgeCloud_hpp

#include <stdio.h>
#include <iostream>
#include <vector>


class SimulatedCentralController;

class SimulatedEdgeCloud{
public:
    SimulatedEdgeCloud(const int addr);
    ~SimulatedEdgeCloud();
    
    void myAddr(const int);
    int myAddr()const;
    void totalWorkload(const double);
    double totalWorkload() const;
    void baseWorkload(const double);
    double baseWorkload() const;
    void writeBaseWorkload();
    void readBaseWorkload();
    void appWorkload(const double);
    double appWorkload() const;
    void totalClientNumber(const int);
    int totalClientNumber() const;
    int reservedClientNumber()const;
    void reservedClientNumber(int);
    void addPotentialClientNumber();
    void substractPotentialClientNumber(int);
    void init(SimulatedCentralController* const, bool const, double hopCost, double baseResp);
    int serviceCount = 0;
    /* event of server */
    void generateBaseWorkload();
    void getConnected(int clientAddr);
    void disconnect(int clientAddr);
    void updateWorkload();
    void reportWorkload();
    void reportConnectedClient();
    double computeResponseTime(int clientAddr);
    void printCloudState();
    
private:
    
    int _myAddr;
    //workload
    double _totalWorkload;
    double _baseWorkload;
    double _appWorkload;
    double _clientLoadWeight = 0.01;
    //client info
    std::vector<int> clientAddr;
    int _totalClientNumber;
    int _totalEdgeCloudNumber;
    int _potentialClient;
    //define basic cost
    double _unitLoadCost;
    double _baseResponse;
    double _singleHopCost;
    
    SimulatedCentralController* _controller;

};
#endif /* SimulatedEdgeCloud_hpp */
