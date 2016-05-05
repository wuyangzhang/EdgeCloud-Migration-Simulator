//
//  ValueIteration.hpp
//  MarkovProcess
//
//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#ifndef ValueIteration_hpp
#define ValueIteration_hpp

#include <stdio.h>

#include <math.h>
#include "MarkovProcess.hpp"
class ValueIteration{
public:
    ValueIteration(MarkovProcess* mdp);
    MarkovProcess* mdp;
    /**
     * The error threshold to stop the iteration
     */
    double epsilon = 1e-4;
    void setError(double epsilon);
    
    int numIterations;
    int getNumberIteration();
    int solve();
    
    enum{
        MIGRATE = 0,
        NO_MIGRATE
    };
    int pattern = 0;
private:
};

#endif /* ValueIteration_hpp */
