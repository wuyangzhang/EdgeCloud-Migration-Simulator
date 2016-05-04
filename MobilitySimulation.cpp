//
//  MobilitySimulation.cpp
//  MarkovProcess
//
//  Created by Wuyang on 5/3/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "MobilitySimulation.hpp"
#include <ctime>

MobilitySimulation::MobilitySimulation(double probabilityLeft, double probabilityStay, double probabilityRight){
    this->probabilityRight = probabilityRight;
    this->probabilityStay = probabilityStay;
    this->probabilityLeft = probabilityLeft;
    
}


MobilitySimulation::~MobilitySimulation(){
    
}


void MobilitySimulation::generateRandomMove(int currentPosition, int pathLength){
    mobilityPath.clear();
    mobilityPath.push_back(currentPosition);
    mobilityPathLength = pathLength;

    srand((unsigned)time(NULL));
    int nextPosition = currentPosition;
    for(int i = 1; i< pathLength; i++){
        double randomIndexAction = rand() / double(RAND_MAX);
        if (randomIndexAction <= probabilityLeft){
            nextPosition = nextPosition -1;
            mobilityPath.push_back(nextPosition);
        }else if(randomIndexAction >probabilityLeft && randomIndexAction <= probabilityStay + probabilityLeft){
            nextPosition = nextPosition;
            mobilityPath.push_back(nextPosition);
        }else{
            nextPosition = nextPosition + 1;
            mobilityPath.push_back(nextPosition);
        }

    }
  
}


void MobilitySimulation::generateRandomMove_circle(int currentPosition, int pathLength, int totalClientPosition){
    mobilityPath.clear();
    mobilityPath.push_back(currentPosition);
    mobilityPathLength = pathLength;
    
    srand((unsigned)time(NULL));
    int nextPosition = currentPosition;
    for(int i = 1; i< pathLength; i++){
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
void MobilitySimulation::printMobilityPath(){
    printf("MobilityPath of total %d paths\n", mobilityPathLength);
    for(vector<int>::iterator it = mobilityPath.begin(); it!=mobilityPath.end();++it){
        printf("%d,",(*it));
    }
    printf("\n");
}