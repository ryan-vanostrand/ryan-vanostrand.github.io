#ifndef _Kitchen_H_
#define _Kitchen_H_
#include "Lock.h"
#include "thread.h"
#include "CV.h"

class Kitchen{
    //lock
    Lock lock;
    //condition variable signalling milk is purchased.
    CV boughtMilk;
    //no need to condition variable when milk is missing for it is in our waitThenDrink function and will not happen, without a thread going to buy milk
    int milk;
    private:
    //private helper variables
    int waitThenDrink();
    void buyMilk();
    public:
    Kitchen();
    ~Kitchen() {};
    //main kitchen function
    int drinkMilkAndBuyIfNeeded();
};
#endif