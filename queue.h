#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "dstructs.h"
#include "list.h"

#define TRUE 1
#define FALSE 0
#define DEFAULT_LIMIT 5000

typedef struct book_order* book_order_p;

/* Queue data structure for a catagory of book orders */
/* Queue is currently implemented with a circular linked list */
struct bo_queue {
  struct bo_list *list;
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
void enqueue(bo_queue queue, book_order_p order);

/* Removes a book order from the front of the queue
 * and returns it. Returns NULL if queue is empty
 */
book_order_p dequeue(bo_queue queue);

/* Functions that checks if the given queue is empty,
 * Returns 1 if empty, 0 otherwise
 */
int is_empty(bo_queue queue);

/* Function determines if the current queue is at its limit 
 */
int is_full(bo_queue queue);

/* Returns the current size of the queue */
size_t get_size(bo_queue queue);

#endif
