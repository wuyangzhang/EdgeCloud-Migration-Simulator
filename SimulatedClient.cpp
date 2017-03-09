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
    
    //setCurrentClientPosition(_startClientPosition);
    _startClientPosition = 0;
    _currentClientPosition = 0;
    
    setMobilityPattern(pleft, pstay, pright);
    _mobilityPath = new vector<int>();
    generateRandomMove_circle();
    writeMove();
    _totalServer = totalServer;
    //generateRandomServer();
    printMobilityPath();
    
}

SimulatedClient::SimulatedClient(int addr, int totalClientPosition, int totalServer){
    
    setMyAddr(addr);
    setTotalClientPosition(totalClientPosition);
    _mobilityPath = new vector<int>();
    _timeSlot = new vector<int>();
    readMove();
    readTimeSlot();
    readMobilityPattern();
    setMobilityPathLength(_mobilityPath->size());
    setStartClientPosition(_mobilityPath->at(0));
    setCurrentClientPosition(_startClientPosition);
    _totalServer = totalServer;
}

SimulatedClient::~SimulatedClient(){
    delete _mobilityPath;
    delete _mobilityPattern;
    delete _timeSlot;
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
    srand(_myAddr % 10);
    //srand(NULL);
    _startClientPosition = rand() % _totalClientPosition;
}

int
SimulatedClient::currentClientPosition(){
    return _currentClientPosition;
}

void
SimulatedClient::updateCurrentLocation_TimeSlot(int timeSlot){
    // return the index of this timeSlot in vector timeSlot
    
    int slotIndex = find(_timeSlot->begin(), _timeSlot->end(),timeSlot) - _timeSlot->begin();
    _currentClientPosition = _mobilityPath->at(slotIndex);
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

vector<double>*
SimulatedClient::getMobilityPattern(){
    return _mobilityPattern;
}

void
SimulatedClient::writeMove(){
    char fileName[200];
    std::sprintf(fileName, "/Users/Wuyang/Dropbox/WINLAB/EdgeCloud/realTrace/roma_taxi/location_copy/location_%d.txt", _myAddr);
    
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
    
    char fileName[200];
    std::sprintf(fileName, "/Users/Wuyang/Desktop/WINLAB/EdgeCloud/realTrace/roma_taxi/location/location_%d.txt", _myAddr);
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
SimulatedClient::readTimeSlot(){
    _timeSlot = new vector<int>();
    char fileName[200];
    std::sprintf(fileName, "/Users/Wuyang/Desktop/WINLAB/EdgeCloud/realTrace/roma_taxi/timeSlot/timeSlot_%d.txt", _myAddr);
    std::ifstream in;
    in.open(fileName);
    
    std::string line;
    
    if (in.is_open())
    {
        while ( getline (in,line) )
        {
            int path = atoi(line.c_str());
            _timeSlot->push_back(path);
        }
        in.close();
    }

}

void
SimulatedClient::readMobilityPattern(){
    
    char fileName[200];
    std::sprintf(fileName, "/Users/Wuyang/Desktop/WINLAB/EdgeCloud/realTrace/roma_taxi/mobilityPattern/%d_Mobilepattern.txt", _myAddr);
    std::ifstream in;
    in.open(fileName);
    
    std::string line;
    
    if (in.is_open())
    {
        getline (in,line);
        double left = atof(line.c_str());
        
        getline (in,line);
        double stay = atof(line.c_str());
        
        getline (in,line);
        double right = atof(line.c_str());
        
        setMobilityPattern(left, stay, right);
        
        
        in.close();
    }
    _mobilityPattern = new vector<double>();
    
    _mobilityPattern->push_back(_pLeft);
    _mobilityPattern->push_back(_pStay);
    _mobilityPattern->push_back(_pRight);
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

bool
SimulatedClient::firstConnect(){
    return _nextConnectedServerName == -1;
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

void
SimulatedClient::printConnectedServer(){
    for(auto i = 0; i< _connectServerList.size(); ++i){
        printf("[SimulatedClient %d] connected servers: %d\n", _myAddr, _connectServerList.at(i));
    }
}

int
SimulatedClient::queryConnectServer(int queryMethod){
    
    if(queryMethod == Markov){
        double expectedQoS = _controller->predictQoS(_currentClientPosition, connectedServer());
        _predictQoS.push_back(expectedQoS);
        _nextConnectedServerName = _controller->checkOptimalConnectedServer(_currentClientPosition, connectedServer());
    }else if(queryMethod == Load){
        _nextConnectedServerName = _controller->checkLowestLoadServer(_myAddr);
    }else if(queryMethod == NeverMigrate){
        _nextConnectedServerName = _controller->checkNerverMigrateServer(connectedServer());
        //_nextConnectedServerName = connectedServer();
    }else if(queryMethod == Nearest){
        _nextConnectedServerName = _controller->checkNearestServer(_currentClientPosition);
        //_nextConnectedServerName = _currentClientPosition / 5;
    }
    
    return _nextConnectedServerName;
}

int
SimulatedClient::generateRandomServer(){
    srand(_myAddr % 4);
    //srand(NULL);
    return rand() % 8;
}

void
SimulatedClient::connectServer(int server){
    connectedServer(server); // set current connected server
    _connectServerList.push_back(server); // add this server to the list of connected servers
    _cloudList->at(server)->getConnected(_myAddr); // ask this server to get connected
}

void
SimulatedClient::disconnectServer(int server){
    _cloudList->at(server)->disconnect(_myAddr);
}

double
SimulatedClient::computeResponseTime(){
    _currentComputeTime =_cloudList->at(connectedServer())->computeResponseTime(_currentClientPosition);
    _computeTime.push_back(_currentComputeTime);
    return _currentComputeTime;
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
        printf("path %d, server %d, predictQoS %f, computeTime %f\n" ,_mobilityPath->at(i), _connectServerList.at(i), _predictQoS.at(i),_computeTime.at(i));
        
        //printf("path %d, server %d, computeTime %f\n" ,_mobilityPath->at(i), _connectServerList.at(i),_computeTime.at(i));
    }
}

void
SimulatedClient::printPredictQoS(){
    printf("[SimulatedClient %d] print predicted response time:\n", _myAddr);
    for(auto i = 0; i < _predictQoS.size(); i++){
        //printf("\tconnection %d, location %d, to cloud %d, consumes %f\n",i, _mobilityPath->at(i) ,_connectServerList.at(i), _computeTime.at(i));
        printf("path %d, server %d, predictQoS %f, computeTime %f\n" ,_mobilityPath->at(i), _connectServerList.at(i), _predictQoS.at(i),_computeTime.at(i));
        
        //printf("path %d, server %d, computeTime %f\n" ,_mobilityPath->at(i), _connectServerList.at(i),_computeTime.at(i));
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
