#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Server.h"
#include "thread.h"
//requests a number of chopsticks
void fight(int n);
//returns all chopsticks given to them currently.
void recede(int n);
Server *gideon = new Server();
thread_t philosophers[10];

int main(int argc, char **argv){
    
    for(int i = 0; i < gideon->philosophers; i++){
        //my philosophers are building weaponry to fight one another.
        printf("Philosopher %d entered the arena!\n", i);
        thread_create(&philosophers[i], &fight, i);
    }
    thread_join(philosophers[0]);
    for(int i = 0; i < gideon->philosophers; i++){
        printf("Philosopher %d exited the arena!\n", i);
        thread_create(&philosophers[i], &recede, i);
    }
    thread_join(philosophers[gideon->philosophers]);
    return 0;
}

void fight(int n){
    //sets time seed for random
    srand(time(NULL));
    //gets random number
    int chopsticks = rand() % (gideon->getMax(n) - gideon->getAllocated(n)) + 1;
    //declares the number of chopsticks wanted by our philosopher
    printf("Philosopher %d wants %d chopsticks for battle\n", n, chopsticks);
    //requests them from our server
    gideon->request(chopsticks, n);
}
void recede(int n){
    printf("Philosopher %d no longer needs his chopsticks as he's leaving battle\n", n);
    gideon->giveBack(n);
}

