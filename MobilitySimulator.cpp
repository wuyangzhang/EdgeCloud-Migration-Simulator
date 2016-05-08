//
//  MobilitySimulator.cpp
//  MarkovProcess
//
//  Created by Wuyang on 5/7/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "MobilitySimulator.hpp"

MobilitySimulator::MobilitySimulator(int totalClientNumber, int totalClientPosition, int totalCloud){
    _totalClientNumber = totalClientNumber;
    _totalClientPosition = totalClientPosition;
    _totalCloud = totalCloud;
    _readModel = false;
}

MobilitySimulator::~MobilitySimulator(){
    delete _controller;
}

void
MobilitySimulator::simulate(){
    MarkovProcess* mdp = new MarkovProcess(_totalCloud,_totalClientPosition);
    mdp->setGamma(0);
    _controller = new SimulatedCentralController(mdp);
    
    //initialize edge clouds
    for(int i = 0; i < _totalCloud; i++){
        SimulatedEdgeCloud* cloud = new SimulatedEdgeCloud(i);
        _controller->cloudList()->push_back(cloud);
    }
    for(vector<SimulatedEdgeCloud*>::iterator it = _controller->cloudList()->begin(); it != _controller->cloudList()->end(); ++it){
        (*it)->init(_controller,_readModel);
    }
    
    //initialize clients
    for(int i = 0; i < _totalClientNumber; i++){
        SimulatedClient* client;
        if(!_readModel)
            client= new SimulatedClient(i, _totalClientPosition, _totalCloud, 10 ,0.8, 0.1, 0.1);
        else
            client= new SimulatedClient(i, _totalClientPosition, _totalCloud);
        
        _controller->clientList()->push_back(client);
        client->init(_controller->cloudList(), _controller);
        client->printMobilityPath();
    }
    
    //start simulate mobility
    
    for(auto i = 0; i < _controller->cloudList()->size(); i++){
        _controller->cloudList()->at(i)->reportWorkload();
    }
    
    //update Markov Decision
    _controller->runMarkovDecision();
    
    //client connect to server first time
    for(auto i = 0; i < _controller->clientList()->size(); i++){
        SimulatedClient* client = _controller->clientList()->at(i);
        client->connectServer(client->connectedServer());
        client->queryConnectServer();
        client->computeResponseTime();
        client->moveToNextClientPosition();
    }
    
    int step = 0;
    while( !_controller->clientList()->back()->terminateMove()){
        
        step ++;
        printf("step %d \n", step);
        
        for(auto i = 0; i < _controller->cloudList()->size(); i++){
            //_controller->cloudList()->at(i)->printConnectedClient();
        }
       
        //after all connection, server report workload
        for(auto i = 0; i < _controller->cloudList()->size(); i++){
            _controller->cloudList()->at(i)->reportWorkload();
            _controller->cloudList()->at(i)->printCloudState();
        }
        
        //update Markov Decision
        _controller->runMarkovDecision();
        
        //client query the connected server
        for(auto i = 0; i < _controller->clientList()->size(); i++){
            SimulatedClient* client = _controller->clientList()->at(i);
            client->disconnectServer(client->connectedServer());
            client->connectServer(client->migrateServer());
            client->queryConnectServer();
            client->computeResponseTime();
            client->moveToNextClientPosition();
        }

    }
    
    /*
    for(auto i = 0; i < _controller->clientList()->size(); i++){
        SimulatedClient* client = _controller->clientList()->at(i);
        client->printComputeTime();
    }
    */
}


