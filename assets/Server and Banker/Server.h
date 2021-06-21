#ifndef _Server_H_
#define _Server_H_
#include "Lock.h"
#include "thread.h"
#include "CV.h"

class Server{
    
    

    private:
    //lock
    Lock lock;
    //condition variable declaring finished philosopher/waiting for one
    CV philosopherFinished;
    
    int chopsticksLeft;
    int max[10];
    int allocated[10];
    //private helper functions
    bool isSafe();
    bool wouldBeSafe(int wantedChopsticks, int threadID);
    
    public:
    int getMax(int n){return max[n];};
    int getAllocated(int n){return allocated[n];};
    int initialChopsticks = 10;
    int philosophers = 10;
    Server();
    ~Server() {};
    //main Server function, returns the number of chopsticks available that we could give to that thread.
    void request(int wantedChopsticks, int threadID);
    void giveBack(int threadID);
};
#endif