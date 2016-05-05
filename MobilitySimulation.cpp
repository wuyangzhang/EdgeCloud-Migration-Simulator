//
//  MobilitySimulation.cpp
//  MarkovProcess
//
//  Created by Wuyang on 5/3/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "MobilitySimulation.hpp"
#include <ctime>

MobilitySimulation::MobilitySimulation(const int totalNumberOfEdgeCloud, const int totalNumberOfClientsPosition, const int pathLength){
    
    
    srand((unsigned)time(NULL));
    mobilityPathLength = pathLength;
    totalClientPosition = totalNumberOfClientsPosition;
    
    //generateRandomMove_circle();
    setRandomMove_circle();
    startClientPosition = mobilityPath.at(0);

    currentClientPosition = startClientPosition;
    currentMoveStep = 0;
    terminateMove = false;
    
    /* server */
    totalEdgeCloudNumber = totalClientPosition;
    
    /* generate a random serviceLocation */
    currentEdgeServiceLocation = rand() % totalEdgeCloudNumber;
    baseResponse = 100;
    singleHopCost = 100;
    unitLoadCost = 100;
}


MobilitySimulation::~MobilitySimulation(){
    
}


void
MobilitySimulation::setMobilePattern(double pLeft, double pStay, double pRight){
    this->probabilityLeft = pLeft;
    this->probabilityStay = pStay;
    this->probabilityRight = pRight;
}


void
MobilitySimulation::generateRandomMove_line(){
    mobilityPath.clear();
    mobilityPath.push_back(currentClientPosition);

    srand((unsigned)time(NULL));
    int nextPosition = currentClientPosition;
    for(int i = 1; i< mobilityPathLength; i++){
        double randomIndexAction = rand() / double(RAND_MAX);
        if (randomIndexAction <= probabilityLeft){
            if(nextPosition ==0)
                nextPosition = 0;
            else
                nextPosition = nextPosition -1;
            mobilityPath.push_back(nextPosition);
        }else if(randomIndexAction >probabilityLeft && randomIndexAction <= probabilityStay + probabilityLeft){
            nextPosition = nextPosition;
            mobilityPath.push_back(nextPosition);
        }else{
            if(nextPosition == totalClientPosition -1)
                nextPosition = totalClientPosition -1;
            else
                nextPosition = nextPosition + 1;
            mobilityPath.push_back(nextPosition);
        }

    }
  
}


void
MobilitySimulation::generateRandomMove_circle(){
    mobilityPath.clear();
    mobilityPath.push_back(currentClientPosition);
    
    srand((unsigned)time(NULL));
    int nextPosition = currentClientPosition;
    for(int i = 1; i< mobilityPathLength; i++){
        double randomIndexAction = rand() / double(RAND_MAX);
        if (randomIndexAction <= probabilityLeft){
            if(nextPosition == 0)
                nextPosition = totalClientPosition -1;
            else
                nextPosition = nextPosition -1;
            mobilityPath.push_back(nextPosition);
        }else if(randomIndexAction >probabilityLeft && randomIndexAction <= probabilityStay + probabilityLeft){
            mobilityPath.push_back(nextPosition);
        }else{
            if(nextPosition == totalClientPosition -1)
                nextPosition = 0;
            else
                nextPosition = nextPosition + 1;
            mobilityPath.push_back(nextPosition);
        }        
    }
}


void
MobilitySimulation::setRandomMove_circle(){
    mobilityPath.clear();
    int pathArray [] = {0,5,4,3,4};
    for(int i = 0; i< 5; i++){
        mobilityPath.push_back(pathArray[i]);
    }
}


void
MobilitySimulation::writeMove(){
    ofstream out;
    out.open ("./moveSample");
    
    for(vector<int>::iterator it = mobilityPath.begin(); it!= mobilityPath.end(); ++it){
        out << (*it)<<"\n";
    }
    
    out.close();

}


void
MobilitySimulation::readMove(){
    mobilityPath.clear();
    
    ifstream in;
    in.open("./moveSample");
    string line;
    
    if (in.is_open())
    {
        while ( getline (in,line) )
        {
            int path = atoi(line.c_str());
            mobilityPath.push_back(path);
        }
        in.close();
    }

    
}


void
MobilitySimulation::printMobilityPath(){
    printf("[MobilitySimulation] MobilityPath of total %d paths\n", mobilityPathLength);
    for(vector<int>::iterator it = mobilityPath.begin(); it!=mobilityPath.end();++it){
        printf("%d,",(*it));
    }
    printf("\n");
}


void
MobilitySimulation::moveToNextClientPosition(){
    
    if(this->terminateMove == true){
        return;
    }
    
    currentMoveStep++;
    
    if(currentMoveStep == mobilityPathLength){
        terminateMove = true;
        return;
    }
    
    currentClientPosition = mobilityPath.at(currentMoveStep);
    
   

}


/* edge cloud server */
vector<double>
MobilitySimulation::generateRandomCloudLoad(){
    srand((unsigned)time(NULL));

    for(int i = 0; i < totalEdgeCloudNumber; i++){
        double workload = rand() / double(RAND_MAX);
        cloudLoad.push_back(workload);
    }
    return cloudLoad;
}


void
MobilitySimulation::writeCloudLoad(){
    ofstream out;
    out.open ("./loadSample");
    
    for(vector<double>::iterator it = cloudLoad.begin(); it!= cloudLoad.end(); ++it){
        out << (*it)<<"\n";
    }
    
    out.close();
}

void
MobilitySimulation::readCloudLoad(){
    cloudLoad.clear();
    
    ifstream in;
    in.open("./loadSample");
    string line;
    
    if (in.is_open())
    {
        while ( getline (in,line) )
        {
            double load = atof(line.c_str());
            cloudLoad.push_back(load);
        }
        in.close();
    }
}


void
MobilitySimulation::printCloudLoad(){
    for(int i = 0; i < cloudLoad.size(); i++){
        printf("[MobilitySimulation] cloud %d has load %f\n", i, cloudLoad.at(i));
    }
}


void
MobilitySimulation::calculateResponseTime_circle(){
    /* need know the location of edge cloud posiiton, load, location of client */
    currentLoad = cloudLoad.at(currentEdgeServiceLocation);
    
    /* calculate cost */
    int clientCloudDistance = std::abs(currentClientPosition - currentEdgeServiceLocation);
    clientCloudDistance =  min(clientCloudDistance, totalEdgeCloudNumber - clientCloudDistance);
    currentReponseTime = baseResponse + singleHopCost * clientCloudDistance + currentLoad / 0.1 * unitLoadCost;
    this->responseTime.push_back(currentReponseTime);
}

double
MobilitySimulation::calcuateTotalResponseTime(){
    double sum = 0;
    for (vector<double>::iterator it = responseTime.begin(); it!= responseTime.end(); ++it){
        sum += *it;
    }
    printf("[MobilitySimulation] total response time is %f ms\n", sum);
    return sum;
}


void
MobilitySimulation::setServiceLocation(){
    currentEdgeServiceLocation = mdp->getAction(currentEdgeServiceLocation, currentClientPosition);
}


void
MobilitySimulation::setServiceLocation(int location){
    currentEdgeServiceLocation = location;
}


void
MobilitySimulation::simulateMove(int model){
       
    printCloudLoad();
    while(!terminateMove){
        calculateResponseTime_circle();
        // update service location
        printf("[MobilitySimulation] client position %d, service position %d, server load %f, responseTime %f\n",currentClientPosition, currentEdgeServiceLocation, currentLoad, currentReponseTime);
        
        setServiceLocation();
        moveToNextClientPosition();

    }
}
