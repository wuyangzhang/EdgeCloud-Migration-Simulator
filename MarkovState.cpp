//
//  MarkovState.cpp
//  MarkovProcess
//
//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "MarkovState.hpp"
//
//  Action.hpp
//  PointerPractice1
//
//  Created by Wuyang on 3/31/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

MarkovState::MarkovState(){}
MarkovState::~MarkovState(){}

MarkovState::MarkovState(int positionOfEdgeCloud, int positionOfMobileUser){
    this->positionOfEdgeCloud = positionOfEdgeCloud;
    this->positionOfMobileUser = positionOfMobileUser;
}


void MarkovState::setTerminate(){
    this->terminate = true;
}

bool MarkovState::isTerminate(){
    return this->terminate;
}

int MarkovState::getPositionOfEdgeCloud(){
    return this->positionOfEdgeCloud;
}

int MarkovState::getPositionOfMobileUser(){
    return this->positionOfMobileUser;
}

double MarkovState::getCostOfMigration(){
    return this->costOfMigration;
}

double MarkovState::getCostOfTransmission(){
    return this->costOfTransmission;
}

double MarkovState::getCostOfTotal(){
    return this->costOfMigration + this->costOfTransmission;
}

double MarkovState::getUtility(){
    return this->utility;
}

void MarkovState::setUtility(double utility){
    this->utility = utility;
}

