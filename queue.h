#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "dstructs.h"
#include "list.h"

#define TRUE 1
#define FALSE 0


typedef struct book_order* book_order;
typedef struct bo_list* bo_list;

/* Queue data structure for a catagory of book orders */
struct bo_queue {
  bo_list list;
  size_t size;
  size_t max_size;

  int isopen;
  pthread_mutex_t mutex;
  pthread_cond_t space_available;
  pthread_cond_t data_available;
};

typedef struct bo_queue* bo_queue;

/* Initalizes a queue for book order structures
 */
bo_queue bo_queue_init(size_t limit);

/* Deconstructor for a bo_queue */
void bo_queue_dec(bo_queue queue);

/* Adds a book order to the back of the queue
 */
void enqueue(bo_queue queue, book_order order);

/* Removes a book order from the front of the queue
 * and returns it. Returns NULL if queue is empty
 */
book_order dequeue(bo_queue queue);

/* Functions that checks if the given queue is empty,
 * Returns 1 if empty, 0 otherwise
 */
int is_empty(bo_queue queue);

/* Returns the current size of the queue */
size_t get_size(bo_queue queue);

#endif
