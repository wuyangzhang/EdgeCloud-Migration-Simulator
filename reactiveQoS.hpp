//
//  reactiveQoS.hpp
//  MarkovProcess
//
//  Created by Wuyang on 5/30/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#ifndef reactiveQoS_hpp
#define reactiveQoS_hpp

#include <stdio.h>
#include <iostream>

class ReactiveQoS{
public:
    ReactiveQoS();
    ~ReactiveQoS();
    //int clientLocation;
    //int edgeCloudLocation;
    double _baseResponse;
    double _singleHopCost;
    int _totalEdgeCloudNumber;
    double _thredshold_QoS;
    double calculateQoS(int, int, int);
    bool exceedQoSThreshold(int, int, int);
};
#endif /* reactiveQoS_hpp */
