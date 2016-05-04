//
//  MarkovAction.cpp
//  MarkovProcess
//
//  Created by Wuyang on 4/5/16.
//  Copyright © 2016 Wuyang. All rights reserved.
//

#include "MarkovAction.hpp"

MarkovAction::MarkovAction(){};

MarkovAction::~MarkovAction(){};

MarkovAction::MarkovAction(int indexOfEdgeCloud){
    this->moveToIndexOfEdgeCloud = indexOfEdgeCloud;
}

int MarkovAction::getAction(){
    return this->moveToIndexOfEdgeCloud;
}
