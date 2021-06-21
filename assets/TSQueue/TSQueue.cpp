#include <assert.h>
#include "TSQueue.h"

// TSQueue.cc 
// Thread-safe queue implementation.

// Try to insert an item. If the queue is
// full, return false; otherwise return true.
bool TSQueue::tryInsert(int item) {
    bool success = false;

    lock.acquire(); // begin critical section
    if ((nextEmpty - front) < MAX) {
        items[nextEmpty % MAX] = item;
        nextEmpty++;
        success = true;
    }
    lock.release(); // end critical section
    return success;
}

// Try to remove an item. If the queue is 
// empty, return false; otherwise return true.
bool TSQueue::tryRemove(int *item) {
    bool success = false;

    lock.acquire(); // begin critical section
    if (front < nextEmpty) {
        *item = items[front % MAX];
        front++;
        success = true;
    }
    lock.release(); // end critical section
    return success;
}

// Initialize the queue to empty
// and the lock to free.
TSQueue::TSQueue() {
    front = nextEmpty = 0;
}







