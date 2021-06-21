#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "thread.h"
#include "Kitchen.h"
const int MAX = 6;
/* Honestly just didn't know what to name the roommate worker method, here we are*/
void *getMilked(void *n){
    //Kitchen pointer conversion
    Kitchen *malk = (Kitchen *) n;
    //calling my class function
    for(int i = 0; i < 100; i++){
        malk->drinkMilkAndBuyIfNeeded();
    }
    return 0;
}
int main(int argc, char **argv){
    thread_t roommates[MAX];
    //seeding random in our main
    srand(time(NULL));
    Kitchen *malk = new Kitchen();
    //creating roommates threads
    for(int i = 0; i < MAX; i++){
        thread_create_p(&roommates[i], getMilked, malk);
    }
    //joining the threads
    for(int i = 0; i < MAX; i++){
        thread_join(roommates[i]);
    }
}
