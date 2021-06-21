#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "CV.h"
#include "Kitchen.h"

int
Kitchen::drinkMilkAndBuyIfNeeded(){
    //using a variable to keep track of whether or not milk was purchased. Don't really use it tho.
    int purchased = 0;
    int iShouldBuy = waitThenDrink();
    if(iShouldBuy){
        purchased = 1;
        buyMilk();
    }
    /*int random = rand() % 100;
    lock.acquire();
    if(random >= 20){
        printf("drank milk happily\n");
        lock.release();
        return 0;
    }
    else{
        printf("Milk empty\n");
        milk = 0;
        milk++;
        lock.release();
        printf("bought milk\n");
        return 1;
    }
    Was a basic lock class for better understanding of how we should use the Condition Variables.
    */
    return purchased;
}
int
Kitchen::waitThenDrink(){
    //acquiring lock like a good programmer, released in the if and the else statement before returns.
    lock.acquire();
    while(milk == 0){
        //waiting for milk to be purchased.
        printf("waiting for milk");
        boughtMilk.wait(&lock);
    }
    int random = rand() % 100;
    if(random >= 20){
        //eighty percent chance of this happening given numbers 0-99.
        printf("drank milk without buying\n");
        lock.release();
        return 0;
    }
    else{
        //twenty percent of this happening. flips to 0 and returns 1 to signal milk needs to be purchased.
        milk = 0;
        printf("out of milk\n");
        lock.release();
        return 1;
    }
}

void
Kitchen::buyMilk(){
    //acquiring lock like a good programmer does, release at bottom of function
    lock.acquire();
    printf("buying milk\n");
    //flips milk
    milk = 1;
    //tell thread I bought milk.
    boughtMilk.signal();
    lock.release();
}

Kitchen::Kitchen(){
    //boring constructor.
    milk = 1;
}