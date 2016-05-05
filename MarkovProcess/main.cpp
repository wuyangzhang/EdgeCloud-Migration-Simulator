//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.


#include "ValueIteration.hpp"
#include "MobilitySimulation.hpp"
#include <iostream>

int main(){
    int totalNumberOfEdgeCloud = 6;
    int totalNumberOfClientsPosition = 6;
    
    MobilitySimulation ms = MobilitySimulation(totalNumberOfEdgeCloud,totalNumberOfClientsPosition, 10);
    ms.setMobilePattern(0.8, 0.1, 0.1);
    //ms.generateRandomMove_circle();
    //ms.writeMove();
    ms.readMove();
    ms.setServiceLocation(4); // initilize service location
    MarkovProcess* mdp = new MarkovProcess(totalNumberOfEdgeCloud,totalNumberOfClientsPosition,3);
    ms.mdp = mdp;
    //ms.generateRandomCloudLoad();
    //ms.writeCloudLoad();
    ms.readCloudLoad();
    ms.mdp->setWorkload(ms.cloudLoad);
    
    mdp->setGamma(0.5);
    mdp->setTransitionProbability(0.8, 0.1, 0.1);
    
    ValueIteration *vi = new ValueIteration(mdp);
    vi->pattern = 0; // pattern 0: optimal migration, pattern 1: no migrate
    vi->solve();
    //mdp->printOptimizedActions();
    
    ms.printMobilityPath();
    ms.simulateMove(1);
    ms.calcuateTotalResponseTime();
    
    delete mdp;
    delete vi;
    
    return 0;
}
