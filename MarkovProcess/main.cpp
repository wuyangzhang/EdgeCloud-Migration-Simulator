//
//  main.cpp
//  MarkovProcess
//
//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "ValueIteration.hpp"
#include "MobilitySimulation.hpp"
#include <iostream>

int main(){
    int totalNumberOfEdgeCloud = 6;
    int totalNumberOfClientsPosition = 6;
    
    MarkovProcess* mdp = new MarkovProcess(totalNumberOfEdgeCloud,totalNumberOfClientsPosition,3);
    
    printf( "[ValueIterationTest] initialize value iteration %d total edge cloud, %d total available mobile client position\n",mdp->totalNumberOfEdgeCloud,mdp->totalNumberOfClientsPosition );
    mdp->setGamma(0);
    //mdp->setTransitionProbability(0.33, 0.33, 0.33);
    mdp->setTransitionProbability(0.8, 0.1, 0.1);
    mdp->numberOfRequest_session = 300;
    
    ValueIteration *vi = new ValueIteration(mdp);
    vi->setError(1e-4);
    vi->pattern = 1; // pattern 0: optimal migration, pattern 1: no migrate
    vi->solve();
    
    printf( "[ValueIterationTest] finish value iteration! \n");
    
    //mdp->printOptimizedActions();
    //printf("show action of state (2,) %d",mdp->getAction(2,1));
    
    MobilitySimulation ms = MobilitySimulation(0.8,0.1,0.1);
    ms.generateRandomMove_circle(3, 10, 6);
    ms.printMobilityPath();
    
    delete mdp;
    delete vi;
    
    return 0;
}
