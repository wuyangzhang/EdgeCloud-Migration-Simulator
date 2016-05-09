//
//  SimulatedClient.cpp
//  MarkovProcess
//
//  Created by Wuyang on 5/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "SimulatedClient.hpp"
#include "SimulatedEdgeCloud.hpp"
#include <fstream>

SimulatedClient::SimulatedClient(int addr, int totalClientPosition, int totalServer, int pathLength, double pleft, double pstay, double pright){
    
    setMyAddr(addr);
    setTotalClientPosition(totalClientPosition);
    setMobilityPathLength(pathLength);
    generateStartPosition();
    setCurrentClientPosition(_startClientPosition);
    setMobilityPattern(pleft, pstay, pright);
    _mobilityPath = new vector<int>();
    generateRandomMove_circle();
    writeMove();
    _totalServer = totalServer;
    generateRandomServer();
}

SimulatedClient::SimulatedClient(int addr, int totalClientPosition, int totalServer){
    
    setMyAddr(addr);
    setTotalClientPosition(totalClientPosition);
    _mobilityPath = new vector<int>();
    readMove();
    setMobilityPathLength(_mobilityPath->size());
    setStartClientPosition(_mobilityPath->at(0));
    setCurrentClientPosition(_startClientPosition);
    _totalServer = totalServer;
    generateRandomServer();
}

SimulatedClient::~SimulatedClient(){
    delete _mobilityPath;
}

void
SimulatedClient::init(std::vector<SimulatedEdgeCloud *> *cloudList, SimulatedCentralController* controller){
    _cloudList = cloudList;
    _controller = controller;
}

void
SimulatedClient::setMyAddr(int addr ){
    _myAddr = addr;
}

void
SimulatedClient::setStartClientPosition(int start){
    _startClientPosition = start;
}
void
SimulatedClient::generateStartPosition(){
    srand(_myAddr);
    _startClientPosition = rand() % _totalClientPosition;
}

void
SimulatedClient::setMobilityPathLength(const int length) {
    _mobilityPathLength = length;
}

void
SimulatedClient::setTotalClientPosition(const int total){
    _totalClientPosition = total;
}

void
SimulatedClient::setCurrentClientPosition(const int position){
    _currentClientPosition = position;
}

void
SimulatedClient::generateRandomMove_circle(){
    _mobilityPath->clear();
    _mobilityPath->push_back(_currentClientPosition);
    
    srand(_myAddr);
    int nextPosition = _currentClientPosition;
    for(int i = 1; i< _mobilityPathLength; i++){
        double randomIndexAction = rand() / double(RAND_MAX);
        if (randomIndexAction <= _pLeft){
            if(nextPosition == 0)
                nextPosition = _totalClientPosition -1;
            else
                nextPosition = nextPosition -1;
            _mobilityPath->push_back(nextPosition);
        }else if(randomIndexAction >_pLeft && randomIndexAction <= _pStay + _pLeft){
            _mobilityPath->push_back(nextPosition);
        }else{
            if(nextPosition == _totalClientPosition -1)
                nextPosition = 0;
            else
                nextPosition = nextPosition + 1;
            _mobilityPath->push_back(nextPosition);
        }
    }
}

void
SimulatedClient::setMobilityPattern(double pleft,double pstay , double pright){
    _pLeft = pleft;
    _pStay = pstay;
    _pRight = pright;
}

void
SimulatedClient::writeMove(){
    char fileName[100];
    std::sprintf(fileName, "./client/moveSample_%d", _myAddr);
    
    std::ofstream out;
    out.open (fileName);

    for(std::vector<int>::iterator it = _mobilityPath->begin(); it!= _mobilityPath->end(); ++it){
        out << (*it)<<"\n";
    }
    
    out.close();
    
}

void
SimulatedClient::readMove(){
    _mobilityPath->clear();
    
    char fileName[100];
    std::sprintf(fileName, "./client/moveSample_%d", _myAddr);
    std::ifstream in;
    in.open(fileName);
    
    std::string line;
    
    if (in.is_open())
    {
        while ( getline (in,line) )
        {
            int path = atoi(line.c_str());
            _mobilityPath->push_back(path);
        }
        in.close();
    }
    
}

void
SimulatedClient::printMobilityPath(){
    
    printf("[SimulatedClient_%d] MobilityPath of total %d paths\n", _myAddr, _mobilityPathLength);
    for(std::vector<int>::iterator it =_mobilityPath->begin(); it!=_mobilityPath->end();++it){
        printf("%d,",(*it));
    }
    printf("\n");
}

void
SimulatedClient::moveToNextClientPosition(){
    if(this->_terminateMove == true){
        return;
    }
    
    _currentMoveStep++;
    
    if(_currentMoveStep == _mobilityPathLength){
        _terminateMove = true;
        return;
    }
    
    _currentClientPosition = _mobilityPath->at(_currentMoveStep);
    
}

void
SimulatedClient::connectedServer(int servername){
    _currentConnectedServerName = servername;
}

int
SimulatedClient::connectedServer(){
    return _currentConnectedServerName;
}

int
SimulatedClient::migrateServer(){
    return _nextConnectedServerName;
}

int
SimulatedClient::queryConnectServer(int queryMethod){
    
    if(queryMethod == Markov){
        _nextConnectedServerName = _controller->checkOptimalConnectedServer(_currentClientPosition, connectedServer());
    }else if(queryMethod == Load){
        _nextConnectedServerName = _controller->checkLowestLoadServer(_myAddr);;
    }
    
    return _nextConnectedServerName;
}

void
SimulatedClient::generateRandomServer(){
    srand(_myAddr + 2);
    int server = rand() % _totalServer;
    connectedServer(server);
}

void
SimulatedClient::connectServer(int server){
    connectedServer(server);
    _connectServerList.push_back(server);
    _cloudList->at(server)->getConnected(_myAddr);
}

void
SimulatedClient::disconnectServer(int server){
    _cloudList->at(server)->disconnect(_myAddr);
}

void
SimulatedClient::computeResponseTime(){
    _currentComputeTime =_cloudList->at(connectedServer())->computeResponseTime(_currentClientPosition);
    _computeTime.push_back(_currentComputeTime);
}

bool
SimulatedClient::terminateMove(){
    return _terminateMove;
}

void
SimulatedClient::printComputeTime(){
    printf("[SimulatedClient %d] print compute time:\n", _myAddr);
    for(auto i = 0; i < _computeTime.size(); i++){
        //printf("\tconnection %d, location %d, to cloud %d, consumes %f\n",i, _mobilityPath->at(i) ,_connectServerList.at(i), _computeTime.at(i));
        printf("[%d,%d], " ,_mobilityPath->at(i), _connectServerList.at(i));
    }
}

double
SimulatedClient::computeAverageTime(){
    double sum = 0;
    for(auto i = 0; i < _computeTime.size(); i++){
        sum += _computeTime.at(i);
    }
    return sum / _computeTime.size();
}

void
SimulatedClient::printAverageTime(){
    printf("[SimulatedClient %d] average processing time of %d requests is %f\n", _myAddr ,_computeTime.size(), computeAverageTime());
}