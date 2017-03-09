//
//  ValueIteration.cpp
//  MarkovProcess
//
//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "ValueIteration.hpp"


ValueIteration::ValueIteration(MarkovProcess* mdp){
    this->mdp = mdp;
}

void ValueIteration::setError(double epsilon){
    this->epsilon = epsilon;
}

int ValueIteration::solve(){
    
    double gamma = mdp->getGamma();
    
    gamma = 0.5;
    
    double threshold;
    
    if(gamma == 1){
        threshold = epsilon;
    }else{
        threshold = epsilon * (1 - gamma) /gamma;
    }
    

    bool finished = false;
    
    numIterations = 0;
    
    /* main part of Value Iteration */
    while(finished != true){
        double maxError = 0;
        for(MarkovState* state = mdp->getStartState(); state != nullptr; state = mdp->getNextState()){
            
            double utility = mdp->getUtility(state);
            double maxCurrentUtil = -1e10;
      
            /*
             *	The following while loop computes \MIN_action.sum T(s,a,s') * (cost + y*U(s'))
             */
            for(MarkovAction* action = mdp->getStartAction(); action != nullptr; action = mdp->getNextAction()){

                /* resultingStates record all possible states after an action */
                vector<MarkovState*>* resultingStates;
                
                if(this->pattern == 0)
                    resultingStates = mdp->getTransition(state, action);
                else
                    resultingStates = mdp->getTransition_noMigrate(state, action);
                
                int size = resultingStates->size();
                double nextUtil = 0 ; /* @param nextUtil -> sum T(s,a,s')U(s') */
                
                /* this for loop calculates sum T(s,a,s')U(s') */
                for(int i = 0; i < size; i++){
                    
                    MarkovState* resultState = resultingStates->at(i);
                    //double cost = mdp->calculateTotalCost_circle(state, resultState);
                    double cost = mdp->calculateTotalCost_test(state,resultState);
                    double probility = mdp->getTransitionProbability_circle(state,action,resultState);
                    nextUtil += probility * ( cost + gamma * mdp->getUtility(resultState));
                    
                    /*
                    if(state->positionOfEdgeCloud == 0 && state->positionOfMobileUser == 2){
                        printf("result state[%d,%d] with %f\n",resultState->positionOfEdgeCloud,resultState->positionOfMobileUser, resultState->workload);
                    }
                     */
                    
                }
                /* looking for the max gain */
                
                if(nextUtil > maxCurrentUtil){
                    /*
                     *	set the minUtility and the optimized action for A SINGLE STATE
                     */
                    maxCurrentUtil = nextUtil;
                    mdp->setTempUtility(state, nextUtil);
                    mdp->setAction(state, action);

                }
            
                
            }
            

            /*
             *	compare the new utility with the previous after one round value iteration
             */
            double currentError = maxCurrentUtil - utility;
            
            /*get abs(currentError)*/
            if( currentError < 0 ){
                currentError = 0 - currentError;
            }
            
            if(currentError > maxError){
                maxError = currentError;
            }
      
        } /* finish one round value iteration */
        
        mdp->copyTempUtility();
        numIterations ++;
        
        if(maxError < threshold){
            finished = true;
        }
    }
    
    //set never migrate!
    if(this->pattern ==1){
        for(vector<MarkovState*>::iterator it = mdp->stateOfCloudClient->begin(); it!=mdp->stateOfCloudClient->end();++it){

            mdp->setAction( *it, (*it)->positionOfEdgeCloud);
        }
    }
    
    return numIterations;
}
