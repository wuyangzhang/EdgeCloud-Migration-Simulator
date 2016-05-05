//
//  MarkovProcess.hpp
//  MarkovProcess
//
//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#ifndef MarkovProcess_hpp
#define MarkovProcess_hpp

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "MarkovState.hpp"
#include "MarkovAction.hpp"

using namespace std;
class MarkovProcess{
public:
    MarkovProcess();
    MarkovProcess(int totalNumberOfEdgeCloud, int totalNumberOfClientsPosition, int model);
    ~MarkovProcess();
    int model;
    void printOptimizedActions();

    /**
     define cloud & mobile user states
     */
    int totalNumberOfEdgeCloud;
    int totalNumberOfClientsPosition;
    
    vector<MarkovState*>* stateOfCloudClient; /*  axis (x,y) x->position of cloud, y->position of client; */
    
    unsigned int currentStateIndex;
    
    MarkovState* state1;
    MarkovState* state2;
    MarkovState* state3;
    
    MarkovState* getState(int indexOfEdgeCloud, int indexOfClient);
    MarkovState* getStartState();
    MarkovState* getNextState();    
    
    void setWorkload(vector<double>);
    /**
     define actions
     */
    int currentAction;
    vector<MarkovAction*>* actions;
    
    void setAction(int indexOfEdgeCloud, int indexOfClient, int indexOfAction); /* each state should have an action */
    void setAction(MarkovState* state, MarkovAction* action); /* each state should have an action */
    void setAction(MarkovState* state, int action);
    MarkovAction* getRandomAction(); /* generate a random valid action for testing use */
    MarkovAction* getStartAction();
    MarkovAction* getNextAction();
    int getAction(int positionOfEdgeCloud, int positionOfClient);
    
    
    /**
     define 
                cost && utility
     *
     *
     *
     */
    double calculateTotalCost(MarkovState* state, MarkovAction * const action); /* not requsite to record all cost based on pair of <state, action>*/
    double calculateTotalCost_circle(MarkovState* const previousState, MarkovState* const transitState);
    
    void setCost(int indexOfEdgeCloud, int indexOfClient, double costOfTotal);
    void setCost(int indexOfEdgeCloud, int indexOfClient, double costOfMigration, double costOfTransmission);
    void setCost(MarkovState * state, MarkovAction * const action, double costOfTotal);
    void setCost(MarkovState * state, MarkovAction * const action, double costOfMigration, double costOfTransmission);
    double getTotalCost(MarkovState* state, MarkovAction * const action);
    double getTransmissionCost(MarkovState* state);
    double getMigrationCost(MarkovState* state, MarkovAction * const action);
    
    double singleHopCost = 0;
    double unitWorkloadCost = 0;
    double baseResponse = 0;
    double migrateCost = 0;
    void setUtility(MarkovState* state, double utility);
    void setTempUtility(MarkovState* state, double utility);
    double getUtility(MarkovState * const state);
    void copyTempUtility();
    void printUtility();
    
    
    /**
     define 
                        discount factor
     */
    double gammaDiscountFactor = 0.1;
    void setGamma(double gamma);
    double getGamma();
    
    
    /**
     define 
                        Transition
     
     
    */
    
    double probabilityLeft;
    double probabilityStay;
    double probabilityRight;
    
    struct mobilityProbability{
        double probabilityLeft;
        double probabilityStay;
        double probabilityRight;
    };
    
    struct transitionStateModel{
        MarkovState* previousState;
        MarkovState* transitState;
        MarkovAction* action;
    };
    std::unordered_map<transitionStateModel*, double>* transitionMap;
    
    void setTransitionProbability(double probabilityLeft, double probabilityStay, double probabilityRight);
    void setTransitionProbability(struct mobilityProbability);
    void setTransitionProbability(MarkovState* previousState, MarkovAction* action, MarkovState* transitState, double probability);
    
    double getTransitionProbability(MarkovState* previousState, MarkovAction* action, MarkovState* transitState);
     double getTransitionProbability_circle(MarkovState* previousState, MarkovAction* action, MarkovState* transitState);
    vector<MarkovState*>* getTransition(MarkovState* state, MarkovAction* action);
    vector<MarkovState*>* getTransition_noMigrate(MarkovState* state, MarkovAction* action);
    
    
    /**
        Define 
                        move
     */
    vector<MarkovState>* move(MarkovState* state, MarkovAction* action);
    vector<MarkovState>* move(MarkovState* state, int action);
    vector<MarkovState*>* resultingStates;
    
    int numberOfRequest_session;
    
private:
    
};


#endif /* MarkovProcess_hpp */
