//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.

#include <iostream>

#include "ValueIteration.hpp"
#include "MobilitySimulator.hpp"

#include "MobilitySimulation.hpp"
//writeCLient does not work

int main(){

    MobilitySimulator* ms = new MobilitySimulator(30, 183, 30);
    

    ms->simulate();
    ms->~MobilitySimulator();
    return 0;
}

