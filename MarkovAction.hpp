#ifndef MARKOVACTION_H
#define MARKOVACTION_H

#include <stdio.h>

class MarkovAction{
    /* in the scenario of edge cloud, the action is the service migration to different edge cloud */
public:
    MarkovAction();
    MarkovAction(int indexOfEdgeCloud);
    ~MarkovAction();
    
    
    int totalNumberOfActions;
    int getAction();
    int moveToIndexOfEdgeCloud;
    
private:
};
#endif/* MarkovAction_hpp */

