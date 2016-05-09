//
//  SimulatedClient.hpp
//  MarkovProcess
//
//  Created by Wuyang on 5/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#ifndef SimulatedClient_hpp
#define SimulatedClient_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>

#include "SimulatedCentralController.hpp"

class SimulatedClient{
public:
    SimulatedClient(int addr, int totalClientPosition,int totalServer, int mobilityPathLength,  double pleft, double pstay, double pright);
     SimulatedClient(int addr, int totalClientPosition, int totalServer);
    ~SimulatedClient();
    
    void generateRandomMove_line();
    void generateRandomMove_circle();
    void setRandomMove_circle();
    void writeMove();
    void readMove();
    void printMobilityPath();
    
    void init(std::vector<SimulatedEdgeCloud*>* cloudList, SimulatedCentralController* controller);
    /* move */
    void moveToNextClientPosition();

    /* connect server */
    void connectServer(int);
    void disconnectServer(int);
    void connectedServer(int);
    int connectedServer();
    int migrateServer();
    void computeResponseTime();
    
    void generateRandomServer();
    int queryConnectServer(int queryMethod);
    bool terminateMove();
    void printComputeTime();
    
    double computeAverageTime();
    void printAverageTime();
private:
    int _myAddr;
    void setMyAddr(int);
    int getMyAddr() const;
    int _startClientPosition;
    void setStartClientPosition(int);
    int getStartClientPosition() const;
    void generateStartPosition();
    int _mobilityPathLength;
    void setMobilityPathLength(const int) ;
    int _currentClientPosition;
    void setCurrentClientPosition(const int);
    int _totalClientPosition;
    void setTotalClientPosition(const int);
    int _currentMoveStep = 0 ;
    bool _terminateMove = false;
    std::vector<int>* _mobilityPath;
    double _pLeft, _pStay, _pRight;
    void setMobilityPattern(double, double, double);
    std::vector<SimulatedEdgeCloud*>* _cloudList;
    std::vector<double> _computeTime;
    double _currentComputeTime;
    std::vector<int> _connectServerList;
    int _currentConnectedServerName;
    int _nextConnectedServerName = -1;
    int _totalServer;
    SimulatedCentralController* _controller;
    enum queryMethod{
        Markov = 0,
        Load
    };
};

#endif /* SimulatedClient_hpp */
