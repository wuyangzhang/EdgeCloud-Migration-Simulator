//
//  MarkovState.hpp
//  MarkovProcess
//
//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#ifndef MarkovState_hpp
#define MarkovState_hpp

#include <stdio.h>
#include "MarkovAction.hpp"

class MarkovState{
public:
    MarkovState();
    ~MarkovState();
    MarkovState(int positionOfEdgeCloud, int positionOfMobileUser);
    MarkovState(int positionOfEdgeCloud, int positionOfMobileUser, double costOfTotal, double utility);
    MarkovState(int positionOfEdgeCloud, int positionOfMobileUser, double costOfMigration, double costOfTransmission, double utility);
    
    int getPositionOfEdgeCloud();
    int getPositionOfMobileUser();
    double getCostOfMigration();
    double getCostOfTransmission();
    double getCostOfTotal();
    double getUtility();
    void setUtility(double utility);
    void setTerminate();
    bool isTerminate();
    
    MarkovAction* action;
    int positionOfEdgeCloud;
    int positionOfMobileUser;
    double costOfMigration = 0;
    double costOfTransmission = 0;
    double costOfTotal = 0; /* costOfMigration + costOfTransmission*/
    double utility = 0;
    double tempUtility = 0;
    double workload = 0;
    bool terminate = false; //duplicate parameter
private:
    
};
#endif /* MarkovState_hpp */
