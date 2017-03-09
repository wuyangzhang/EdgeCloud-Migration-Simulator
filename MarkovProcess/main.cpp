//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
// revised in March 09 2017
#include <iostream>

#include "ValueIteration.hpp"
#include "MobilitySimulator.hpp"

#include "MobilitySimulation.hpp"
//writeCLient does not work

int main(){

    MobilitySimulator* ms = new MobilitySimulator(16, 1, 16);
    
    ms->simulate();
    ms->~MobilitySimulator();
    
    return 0;
}

