#include <assert.h>
#include <stdio.h>
#include "thread.h"
#include "TSQueue.h"

void *putSome(void *p);
void testRemoval(TSQueue *tsqueuePtr);

// TSQueueMain.cc
//   Test code for TSQueue.

int main(int argc, char **argv) {
    TSQueue *queues[3];
    thread_t workers[3];
    int i;

    // Start worker threads to insert.
    for (i = 0; i < 3; i++) {
        queues[i] = new TSQueue();
        thread_create_p(&workers[i], putSome, queues[i]);
    }

    // Wait for some items to be put.
    thread_join(workers[0]);

    // Remove 20 items from each queue.
    for (i = 0; i < 3; i++) {
        printf("Queue %d:\n", i);
        testRemoval(queues[i]);
    }
}

// Insert 50 items into a queue.
void *putSome(void *p) {
    TSQueue *queue = (TSQueue *)p;
    int i;

    for (i = 0; i < 50; i++) {
        if (queue->tryInsert(i))
          printf("Inserted %d\n", i);
        else
          printf("Failed to insert %d\n", i);
    }
    return NULL;
}

// Remove 20 items from a queue.
void testRemoval(TSQueue *queue) {
    int i, item;

    for (i = 0; i < 20; i++) {
        if (queue->tryRemove(&item))
          printf("Removed %d\n", item);
        else
          printf("Nothing there.\n");
        }
    }


