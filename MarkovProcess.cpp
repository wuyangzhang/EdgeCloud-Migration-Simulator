//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.

#include "MarkovProcess.hpp"
#include <cmath>

/**
 * Translate two absolute coordinates (r,c) and (r',c') to relative
 * Transition from the coordinate(cloud1, client1) to the coordinate(cloud2, client2)
 * Actions represents moving to all available cloud sites.
 */

MarkovProcess::MarkovProcess(){};
MarkovProcess::MarkovProcess(int totalNumberOfEdgeCloud, int totalNumberOfClientsPosition){
    
    /**
     *
     *	Initiate all states.
     *	All states conditions (cloudPosition, clientPosition)
     *
     */
    this->model = 3;
    this->totalNumberOfEdgeCloud = totalNumberOfEdgeCloud;
    this->totalNumberOfClientsPosition = totalNumberOfClientsPosition;
    
    stateOfCloudClient = new vector<MarkovState*>(); /*  axis (x,y) x->position of cloud, y->position of client; */
    

    for(int i = 0; i < this->totalNumberOfEdgeCloud; i++){
        for(int j = 0; j<this->totalNumberOfClientsPosition; j++){
            MarkovState* state = new MarkovState();
            state->positionOfEdgeCloud = i;
            state->positionOfMobileUser = j;
            state->action = new MarkovAction();
            state->action->moveToIndexOfEdgeCloud = 0;
            state->utility = 0;
            state->tempUtility = 0;
            
            if(i%2!=0){
                state->workload = 0.8;
            }else{
                state->workload = 0.2;
            }
            this->stateOfCloudClient->push_back(state);
        }
    }
    
    this->state1 = new MarkovState();
    this->state2 = new MarkovState();
    this->state3 = new MarkovState();
    
    this->resultingStates = new vector<MarkovState*>();
    
    /**
     *	initiate all actions
     */
    this->actions = new vector<MarkovAction*>();

    for(int i = 0; i< this->totalNumberOfEdgeCloud; i++){
        MarkovAction* action = new MarkovAction();
        action->moveToIndexOfEdgeCloud = i;
        this->actions->push_back(action);
    }
    
    
    /**
     *	initiate transition & probabilty
     */
    this->transitionMap = new std::unordered_map<transitionStateModel*, double>(); /* transitionModel [currentState][Action][nextState] = probablity */
    this->probabilityRight = 0.1;
    this->probabilityLeft = 0.8;
    this->probabilityStay = 0.1;
    
    /*
     * initiate number of request per session, 2min 200 request
     */
    this->numberOfRequest_session = 5 * 60;
    this->singleHopCost = 14;
    this->unitWorkloadCost = 100;
    this->baseResponse = 120;
    this->migrateCost = 2000;
    
}

MarkovProcess::~MarkovProcess(){
   
    
    /* loop actions and state, resulting state, transitMap vector, transitmap to release all pointers */
    for(int i = 0; i < stateOfCloudClient->size(); i++){
        delete stateOfCloudClient->at(i)->action;
        delete stateOfCloudClient->at(i);
    }
    
    for(vector<MarkovAction*>::iterator it = this->actions->begin(); it != this->actions->end(); it++){
        delete *it;
    }
    
  
    for(auto it = this->transitionMap->begin(); it!= this->transitionMap->end(); it++){
        delete it->first;
    }
    
    delete this->transitionMap;
    delete this->actions;
    delete this->stateOfCloudClient;
    delete this->resultingStates;
    
    
}

/**
 ----------------------------------------------------------------------------------------
 
 Define Cost
 ----------------------------------------------------------------------------------------
 */

double
MarkovProcess::calculateWorkloadCost(int totalClientNumber){
    double workloadCost = 0;
    if(totalClientNumber < 10){
        workloadCost = totalClientNumber * 1;
    }else if(totalClientNumber < 12){
        workloadCost = totalClientNumber * 3.5;
    }else if(totalClientNumber < 15){
        workloadCost = totalClientNumber * 14.5;
    }else if(totalClientNumber < 25){
        workloadCost = totalClientNumber * 38.5;
    }else{
        workloadCost = totalClientNumber * 100;
    }
    return workloadCost;
}

double
MarkovProcess::calculateTotalCost_circle(MarkovState* const previousState, MarkovState* const transitState){
    
    //previous location of edge cloud & client
    int previousEdgeCloud = previousState->positionOfEdgeCloud;
    int previousClient = previousState->positionOfMobileUser;
    double previousResponse = 0;

    //transit location of edge cloud & client
    int transitEdgeCloud = transitState->positionOfEdgeCloud;
    int transitClient = transitState->positionOfMobileUser;
    double transitResponse = 0;

    //responseGain, migrationCost, responseGain - migrationCost
    double responseGain = 0;
    double migrateCost = 0;
    double totalGain = 0;

    
    int previousClientDistance = std::abs(previousClient - previousEdgeCloud);
    previousClientDistance = min(previousClientDistance, std::abs(totalNumberOfEdgeCloud - previousClientDistance));
    
    int transitClientDistance = std::abs(transitClient - transitEdgeCloud);
    transitClientDistance = min(transitClientDistance, std::abs(totalNumberOfEdgeCloud - transitClientDistance));
    
    if(this->model == 2){
        if (previousClientDistance < this->totalNumberOfEdgeCloud - previousClientDistance) {
            previousResponse = baseResponse + singleHopCost * previousClientDistance;
        }else{
            previousResponse = baseResponse + singleHopCost * (totalNumberOfEdgeCloud - previousClientDistance);
        }
        
        int transitClientDistance = std::abs(transitClient - transitEdgeCloud);
        if (transitClientDistance < this->totalNumberOfEdgeCloud - transitClientDistance) {
            transitResponse = baseResponse + singleHopCost * transitClientDistance;
        }else{
            transitResponse = baseResponse + singleHopCost * (totalNumberOfEdgeCloud - transitClientDistance);
        }
    }
    
    
    if(this->model == 3){
        if (previousClientDistance < this->totalNumberOfEdgeCloud - previousClientDistance) {
            previousResponse = baseResponse + singleHopCost * previousClientDistance + calculateWorkloadCost(previousState->connectedClient);
        }else{
            previousResponse = baseResponse + singleHopCost * (totalNumberOfEdgeCloud - previousClientDistance) +  calculateWorkloadCost(previousState->connectedClient);

        }
        
        if (transitClientDistance < this->totalNumberOfEdgeCloud - transitClientDistance) {
            transitResponse = baseResponse + singleHopCost  * transitClientDistance +  calculateWorkloadCost(transitState->connectedClient);
        }else{
            transitResponse = baseResponse + singleHopCost  * (totalNumberOfEdgeCloud - transitClientDistance) + calculateWorkloadCost(transitState->connectedClient);

        }
    }
    
    responseGain = (previousResponse - transitResponse) * this->numberOfRequest_session;
    
    if(previousEdgeCloud == transitEdgeCloud){
        migrateCost = 0;
    }else{
        migrateCost = this->migrateCost;
    }

    return totalGain = responseGain - migrateCost;
    
}


double
MarkovProcess::calculateTotalCost(MarkovState* state, MarkovAction * const action){
    
    state->costOfMigration = state->positionOfEdgeCloud - action->moveToIndexOfEdgeCloud;
    
    if(state->costOfMigration < 0 ){
        state->costOfMigration = 0 - state->costOfMigration;
    }
    
    
    /* transmission cost = difference between < new cloud position, client position */
    state->costOfTransmission =  action->moveToIndexOfEdgeCloud - state->positionOfMobileUser;
    
    if(state->costOfTransmission < 0){
        state->costOfTransmission = 0 - state->costOfTransmission;
    }
    
    state->costOfTotal = state->costOfMigration + state->costOfTransmission;
    
    return state->costOfTotal;
}


void
MarkovProcess::setCost(int indexOfEdgeCloud, int indexOfClient, double costOfTotal){
    this->stateOfCloudClient->at(indexOfEdgeCloud * this->totalNumberOfEdgeCloud + indexOfClient)->costOfTotal = costOfTotal;
}


void
MarkovProcess::setCost(int indexOfEdgeCloud, int indexOfClient, double costOfMigration, double costOfTransmission){
    this->stateOfCloudClient->at(indexOfEdgeCloud * this->totalNumberOfEdgeCloud + indexOfClient)->costOfMigration = costOfMigration;
    this->stateOfCloudClient->at(indexOfEdgeCloud * this->totalNumberOfEdgeCloud + indexOfClient)->costOfTransmission = costOfTransmission;
}


void
MarkovProcess::setCost(MarkovState* state, MarkovAction * const action, double costOfTotal){
    state->costOfTotal = costOfTotal;
}


void
MarkovProcess::setCost(MarkovState* state, MarkovAction * const action, double costOfMigration, double costOfTransmission){
    state->costOfMigration = costOfMigration;
    state->costOfTransmission = costOfTransmission;
}


double
MarkovProcess::getTotalCost(MarkovState* state, MarkovAction * const action){
    return state->costOfTotal;
}


double
MarkovProcess::getTransmissionCost(MarkovState* state){
    return state->costOfTransmission;
}


double
MarkovProcess::getMigrationCost(MarkovState* state, MarkovAction * const action){
    return state->costOfMigration;
}


void
MarkovProcess::printOptimizedActions(){
    for(auto i = 0; i < stateOfCloudClient->size(); i++){
        printf("[MarkovProcess] optimized migration site for state (%d, %d) is %d, load %f, utility %f\n", stateOfCloudClient->at(i)->positionOfEdgeCloud, stateOfCloudClient->at(i)->positionOfMobileUser, stateOfCloudClient->at(i)->action->moveToIndexOfEdgeCloud, stateOfCloudClient->at(i)->workload,stateOfCloudClient->at(i)->utility);
    }
}

/**
	State Functions:
 
	MarkovState getState(int indexOfEdgeCloud, int indexOfClient);
	MarkovState getStartState();
	MarkovState getNextState();
	void setTerminateState(int indexOfEdgeCloud, int indexOfClient);
 
 */
MarkovState*
MarkovProcess::getState(int indexOfEdgeCloud, int indexOfClient){
    this->currentStateIndex = 0;
    return this->stateOfCloudClient->at(indexOfEdgeCloud * this->totalNumberOfEdgeCloud + indexOfClient);
}

MarkovState*
MarkovProcess::getStartState(){
    this->currentStateIndex = 0;
    return this->stateOfCloudClient->at(0);
}


MarkovState*
MarkovProcess::getNextState(){
    currentStateIndex++;
    /* if reach to the last state */
    if(currentStateIndex == this->stateOfCloudClient->size())
        return nullptr;
    else
        return this->stateOfCloudClient->at(currentStateIndex);
}


void
MarkovProcess::setWorkload(vector<double> load){
    if(load.size() != totalNumberOfEdgeCloud)
        return;
    
    for(int i = 0; i < totalNumberOfEdgeCloud; i++){
        for(int j = 0; j < totalNumberOfClientsPosition; j++){
            stateOfCloudClient->at(i*totalNumberOfClientsPosition + j)->workload = load.at(i);
        }
    }
}

void
MarkovProcess::setWorkload(int cloudPosition,double load){
    
    for(int j = 0; j < totalNumberOfClientsPosition; j++){
        stateOfCloudClient->at(cloudPosition*totalNumberOfClientsPosition + j)->workload = load;
    }
}

void
MarkovProcess::setConnectedClient(int cloudPosition, int connectedClientNum){
    for(int j = 0; j < totalNumberOfClientsPosition; j++){
        stateOfCloudClient->at(cloudPosition*totalNumberOfClientsPosition + j)->connectedClient = connectedClientNum;
    }
}

void
MarkovProcess::setUtility(MarkovState* state, double utility){
    /* find this state in vector this->stateOfCloudClient and change utility*/
    for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it != this->stateOfCloudClient->end(); it++){
        if ( (*it)->positionOfEdgeCloud == state->positionOfEdgeCloud && (*it)->positionOfMobileUser == state->positionOfMobileUser){
            (*it)->utility = utility;
        }
    }

}
void
MarkovProcess::setTempUtility(MarkovState* state, double tempUtility){
    /* find this state in vector this->stateOfCloudClient and change utility*/
    for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it != this->stateOfCloudClient->end(); it++){
        if ( (*it)->positionOfEdgeCloud == state->positionOfEdgeCloud && (*it)->positionOfMobileUser == state->positionOfMobileUser){
            (*it)->tempUtility = tempUtility;
        }
    }
}

double
MarkovProcess::getUtility(MarkovState * const state){
    /* find this state in vector this->stateOfCloudClient and return utility*/
    for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it != this->stateOfCloudClient->end(); it++){
        if ( (*it)->positionOfEdgeCloud == state->positionOfEdgeCloud && (*it)->positionOfMobileUser == state->positionOfMobileUser){
            return (*it)->utility;
        }
    }
    return 0;
}

void
MarkovProcess::copyTempUtility(){
    for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it != this->stateOfCloudClient->end(); it++){
        (*it)->utility = (*it)->tempUtility;
    }
}

void
MarkovProcess::printUtility(){
    for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it != this->stateOfCloudClient->end(); it++){
        printf("Utility of state(%d,%d): %f\n", (*it)->positionOfEdgeCloud, (*it)->positionOfMobileUser, (*it)->utility);
    }
}

/**
 ----------------------------------------------------------------------------------------
 
 
                                Define Action
 ----------------------------------------------------------------------------------------
 */

/**
 *	Action Functions:
 *
 *	MarkovAction getRandomAction();
 *	void setAction(int indexOfEdgeCloud, int indexOfClient, int action);
 *	void setAction(MarkovState state, MarkovAction action);
 *	MarkovAction getStartAction();
 *	MarkovAction getNextAction();
 *	MarkovAction getAction(MarkovState state);
 */

/* generate a random valid action for testing use */
MarkovAction* MarkovProcess::getRandomAction(){
    srand((unsigned)time(NULL));
    int randomIndexAction = rand() % (this->totalNumberOfEdgeCloud);
    return this->actions->at(randomIndexAction);
}

void
MarkovProcess::setAction(int indexOfEdgeCloud, int indexOfClient, int indexOfAction){
    this->stateOfCloudClient->at(indexOfEdgeCloud * this->totalNumberOfEdgeCloud + indexOfClient)->action = this->actions->at(indexOfAction);
}


void
MarkovProcess::setAction(MarkovState* state, MarkovAction* action){
    /* copy the action pointer */
    int cloudIndex = action->moveToIndexOfEdgeCloud;
    state->action->moveToIndexOfEdgeCloud = cloudIndex;
}

void
MarkovProcess::setAction(MarkovState *state, int action){
    state->action->moveToIndexOfEdgeCloud = action;
}


MarkovAction*
MarkovProcess::getStartAction(){
    this->currentAction = 0;
    return this->actions->at(0);
}


/**
 * Go through all possible actions.
 * @return Next applicable actions, <tt>null</tt> if no more action's possible.
 */
MarkovAction*
MarkovProcess::getNextAction(){
    currentAction ++;
    /* if current action goes to the last actions (action is based on the index of edge cloud)! */
    if(currentAction == this->totalNumberOfEdgeCloud)
        return nullptr;
    else
        return actions->at(currentAction);
}

int
MarkovProcess::getAction(int positionOfEdgeCloud, int positionOfClient){
    /*find this action in the vector*/
    MarkovState* state = new MarkovState(positionOfEdgeCloud, positionOfClient);
    state->action = new MarkovAction();
    for(vector<MarkovState*>::iterator it = stateOfCloudClient->begin(); it!=stateOfCloudClient->end();++it){
        if((*it)->positionOfEdgeCloud== state->positionOfEdgeCloud && (*it)->positionOfMobileUser == state->positionOfMobileUser){
            int action =(*it)->action->moveToIndexOfEdgeCloud;
            delete state->action;
            delete state;
            return action;
        }
        
    }
    delete state->action;
    delete state;
    return -1;
}



/**
 Gamma Functions:
 setGamma()
 getGamma()
 */
void
MarkovProcess::setGamma(double gamma){
    this->gammaDiscountFactor = gamma;
}


double
MarkovProcess::getGamma(){
    return this->gammaDiscountFactor;
}


/**
 ----------------------------------------------------------------------------------------


                                Define Transition
 ----------------------------------------------------------------------------------------
 */


void
MarkovProcess::setTransitionProbability(MarkovState* previousState, MarkovAction* action, MarkovState* transitState, double probability){
    if(previousState->terminate)
        return;
    transitionStateModel* tsm = new transitionStateModel();
    tsm->previousState = previousState;
    tsm->transitState = transitState;
    tsm->action = action;
    
    this->transitionMap->insert(std::make_pair(tsm, probability));
}


void
MarkovProcess::setTransitionProbability(double probabilityLeft, double probabilityStay, double probabilityRight){
    this->probabilityLeft = probabilityLeft;
    this->probabilityStay = probabilityStay;
    this->probabilityRight = probabilityRight;
}


void
MarkovProcess::setTransitionProbability(struct mobilityProbability mp){
    this->probabilityLeft = mp.probabilityLeft;
    this->probabilityStay = mp.probabilityStay;
    this->probabilityRight = mp.probabilityRight;
}


double
MarkovProcess::getTransitionProbability_circle(MarkovState* previousState, MarkovAction* action, MarkovState* transitState){
    /**
     *	Redefine this function based on the cloud scenario
     *	transition probability is constant value based on the pair of <previousState, action, transitState>
     *	transitState = {(action,client-1), (action,client), (action, client + 1)}
     *	transitProbablity for the above three states are correspondingly < p , 1-p-q, q >.
     */
    
    int clientPreviousPosition = previousState->positionOfMobileUser;
    int clientTransitPosition = transitState->positionOfMobileUser;
    int positionDiff = clientTransitPosition - clientPreviousPosition;
    
    /* special condition when client is at the edge position */
    
    if(clientPreviousPosition == 0){
        // diff n-1 -0, 0 -0, 1-0
        if(positionDiff == this->totalNumberOfClientsPosition -1 )
            return this->probabilityLeft;
        if(positionDiff == 0)
            return this->probabilityStay;
        if(positionDiff == 1)
            return this->probabilityRight;
    }
    
    if(clientPreviousPosition == this->totalNumberOfClientsPosition -1){
        // diff 0 - (n-1), 0, -1
        if(positionDiff == 0)
            return this->probabilityStay;
        if(positionDiff == -1)
            return this->probabilityLeft;
        if(positionDiff == -this->totalNumberOfClientsPosition + 1)
            return this->probabilityRight;
    }
    
    
    if(positionDiff == -1){
        return this->probabilityLeft;
    }else if(positionDiff == 0){
        return this->probabilityStay;
    }else{
        return this->probabilityRight;
    }
    
    
}


double
MarkovProcess::getTransitionProbability(MarkovState* previousState, MarkovAction* action, MarkovState* transitState){
    
    int clientPreviousPosition = previousState->positionOfMobileUser;
    int clientTransitPosition = transitState->positionOfMobileUser;
    int positionDiff = clientTransitPosition - clientPreviousPosition;
    
    if(positionDiff == -1){
        return this->probabilityLeft;
    }else if(positionDiff == 0){
        return this->probabilityStay;
    }else{
        return this->probabilityRight;
    }
}


vector<MarkovState*>*
MarkovProcess::getTransition(MarkovState* state, MarkovAction* action){
    this->resultingStates->clear();
    
    // circle topology !
    
    /* consider edge board situations. if client is at the the first or the last edge, the resultings are two (can NOT across board)*/
    
    if(state->positionOfMobileUser == 0){
        
        for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it!=this->stateOfCloudClient->end();++it){
            if((*it)->positionOfEdgeCloud == action->moveToIndexOfEdgeCloud && (*it)->positionOfMobileUser ==0){
                this->state1 =  *it;
            }else if((*it)->positionOfEdgeCloud == action->moveToIndexOfEdgeCloud && (*it)->positionOfMobileUser ==1){
                this->state2 =  *it;
            }else if((*it)->positionOfEdgeCloud == action->moveToIndexOfEdgeCloud && (*it)->positionOfMobileUser == this->totalNumberOfClientsPosition -1){
                this->state3 =  *it;
            }
        }
       
        this->resultingStates->push_back(this->state1);
        this->resultingStates->push_back(this->state2);
        this->resultingStates->push_back(this->state3);
        
        return this->resultingStates;
        
    }else if(state->positionOfMobileUser == this->totalNumberOfClientsPosition -1){
        
        for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it!=this->stateOfCloudClient->end();++it){
            if((*it)->positionOfEdgeCloud == action->moveToIndexOfEdgeCloud && (*it)->positionOfMobileUser == state->positionOfMobileUser){
                this->state1 =  *it;
            }else if((*it)->positionOfEdgeCloud == action->moveToIndexOfEdgeCloud && (*it)->positionOfMobileUser ==state->positionOfMobileUser-1){
                this->state2 =  *it;
            }else if((*it)->positionOfEdgeCloud == action->moveToIndexOfEdgeCloud && (*it)->positionOfMobileUser == 0){
                this->state3 =  *it;
            }
        }
        
        this->resultingStates->push_back(this->state1);
        this->resultingStates->push_back(this->state2);
        this->resultingStates->push_back(this->state3);
        return this->resultingStates;
        
    }else{
        for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it!=this->stateOfCloudClient->end();++it){
            if((*it)->positionOfEdgeCloud == action->moveToIndexOfEdgeCloud && (*it)->positionOfMobileUser == state->positionOfMobileUser){
                this->state1 =  *it;
            }else if((*it)->positionOfEdgeCloud == action->moveToIndexOfEdgeCloud && (*it)->positionOfMobileUser ==state->positionOfMobileUser-1){
                this->state2 =  *it;
            }else if((*it)->positionOfEdgeCloud == action->moveToIndexOfEdgeCloud && (*it)->positionOfMobileUser == state->positionOfMobileUser + 1){
                this->state3 =  *it;
            }
        }
        
        this->resultingStates->push_back(this->state1);
        this->resultingStates->push_back(this->state2);
        this->resultingStates->push_back(this->state3);
        
        return this->resultingStates;
        
    }
}

vector<MarkovState*>*
MarkovProcess::getTransition_noMigrate(MarkovState* state, MarkovAction* action){
    this->resultingStates->clear();
    
    if(state->positionOfMobileUser == 0){
        
        for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it!=this->stateOfCloudClient->end();++it){
            if((*it)->positionOfEdgeCloud == state->positionOfEdgeCloud){
                if((*it)->positionOfMobileUser ==0)
                    this->state1 = *it;
                else if((*it)->positionOfMobileUser ==1)
                    this->state2 = *it;
                else if((*it)->positionOfMobileUser ==totalNumberOfClientsPosition -1)
                    this->state3 = *it;
            }
        }
                
        this->resultingStates->push_back(this->state1);
        this->resultingStates->push_back(this->state2);
        this->resultingStates->push_back(this->state3);
        
        return this->resultingStates;
        
    }else if(state->positionOfMobileUser == this->totalNumberOfClientsPosition -1){
        
        for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it!=this->stateOfCloudClient->end();++it){
            if((*it)->positionOfEdgeCloud == state->positionOfEdgeCloud){
                if((*it)->positionOfMobileUser == state->positionOfMobileUser)
                    this->state1 = *it;
                else if((*it)->positionOfMobileUser == state->positionOfMobileUser -1)
                    this->state2 = *it;
                else if((*it)->positionOfMobileUser == 0)
                    this->state3 = *it;
            }
        }
        
        this->resultingStates->push_back(this->state1);
        this->resultingStates->push_back(this->state2);
        this->resultingStates->push_back(this->state3);
        return this->resultingStates;
        
    }else{
        
        for(vector<MarkovState*>::iterator it = this->stateOfCloudClient->begin(); it!=this->stateOfCloudClient->end();++it){
            if((*it)->positionOfEdgeCloud == state->positionOfEdgeCloud){
                if((*it)->positionOfMobileUser == state->positionOfMobileUser)
                    this->state1 = *it;
                else if((*it)->positionOfMobileUser == state->positionOfMobileUser -1)
                    this->state2 = *it;
                else if((*it)->positionOfMobileUser == state->positionOfMobileUser + 1)
                    this->state3 = *it;
            }
        }
        
        this->resultingStates->push_back(this->state1);
        this->resultingStates->push_back(this->state2);
        this->resultingStates->push_back(this->state3);
        
        return this->resultingStates;
        
    }


}


