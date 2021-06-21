#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "CV.h"
#include "Lock.h"
#include "Server.h"

Server::Server(){
    //seeding our random function.
    srand(time(NULL));
    for(int i = 0; i < philosophers; i++){
        max[i] = rand() % (initialChopsticks - 1) + 2;
        allocated[i] = 0;
        chopsticksLeft = initialChopsticks;
    }
}
void
Server::request(int wantedChopsticks, int threadID){
    //simple resource management setup from book.
    //grabs lock
    lock.acquire();
    //asserts current safestate
    assert(isSafe());
    //check if this would be a safe transaction
    while(!wouldBeSafe(wantedChopsticks, threadID)){
        //waits for a stick
        philosopherFinished.wait(&lock);
    }
    //grants that philosopher their stick
    allocated[threadID] += wantedChopsticks;
    chopsticksLeft -= wantedChopsticks;
    //asserts safety
    assert(isSafe());
    //releases lock.
    lock.release();
}
bool
Server::isSafe(){
    //keeps track of available potential chopsticks in banker's algorithm
    int potentialChopsticks = chopsticksLeft;
    //will keep track of the thread number that can complete throughout.
    int safeThreadNum;
    //chopsticks that will be needed by philosophers soon.
    int eventuallyNeededChopsticks[philosophers];
    //initializing these.
    for(int i = 0; i < philosophers; i++){
        eventuallyNeededChopsticks[i] = max[i] - allocated[i];
    }
    //keep track of which philosophers can have their need met.
    bool philosopherFinish[philosophers];
    //initializing.
    for(int i = 0; i < philosophers; i++){
        philosopherFinish[i] = false;
    }
    //we want a loop that never breaks unless our conditions are met.
    while(true){
        //this helps to see if there is no longer a thread that holds our conditions.
        safeThreadNum = -1;
        //loop looking for a safe thread.
        printf("searching for safe thread\n");
        for(int i = 0; i < philosophers; i++){
            if(eventuallyNeededChopsticks[i] < potentialChopsticks && !philosopherFinish[i]){
                safeThreadNum = i;
            }
        }
        printf("safethread is %d, or -1 if none remain with a finish of false.\n", safeThreadNum);
        if(safeThreadNum == -1){
            //ensuring all threads could finish.
            printf("ensuring all threads are safe\n");
            for(int i = 0; i < philosophers; i++){
                if(!philosopherFinish[i]){
                    safeThreadNum = i;
                }
            }
            //all threads can finish if there was no false above.
            if(safeThreadNum == -1){
                printf("we're safe\n");
                return true;
            }
            else{
                printf("we're not safe\n");
                return false;
            }
        }
        else{
            //if this is reached our thread can finish, so we can reallocate those resources safely.
            printf("adding the chopsticks %d of the safe thread to our potential\n", allocated[safeThreadNum]);
            philosopherFinish[safeThreadNum] = true;
            potentialChopsticks += allocated[safeThreadNum];
        }
    }
}
bool
Server::wouldBeSafe(int wantedChopsticks,int threadID){
    bool result = false;
    //changing values
    chopsticksLeft-= wantedChopsticks;
    allocated[threadID] += wantedChopsticks;
    //checking Safety
    if (isSafe()){
        result = true;
    }
    //changing values back
    chopsticksLeft+= wantedChopsticks;
    allocated[threadID] -= wantedChopsticks;
    //returning the boolean
    return result;
}
void
Server::giveBack(int threadID){
    lock.acquire();
    chopsticksLeft += allocated[threadID];
    allocated[threadID] = 0;
    philosopherFinished.broadcast();
    lock.release();
}