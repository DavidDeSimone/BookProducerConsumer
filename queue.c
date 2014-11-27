#include "queue.h"

bo_queue bo_queue_init(size_t limit) {
  bo_queue queue = malloc(sizeof(struct bo_queue));
  
  /* Create the empty order list */
  queue->list = list_init();
  queue->size = 0;
  queue->isopen = TRUE;
  queue->max_size = limit;

  /* Initalize mutex and condition variables */
  pthread_mutex_init (&queue->mutex, 0);
  pthread_cond_init(&queue->space_available, 0);
  pthread_cond_init(&queue->data_available, 0);

  return queue;
}


void bo_queue_dec(bo_queue queue) {
  if(queue == NULL) {
    return;
  }

  list_dec(queue->list);
  pthread_mutex_destroy(&queue->mutex);
  pthread_cond_destroy(&queue->space_available);
  pthread_cond_destroy(&queue->data_available);

  free(queue);
}

void enqueue(bo_queue queue, book_order order) {
  /* Put a lock on the queue */
  pthread_mutex_lock(&queue->mutex);
  
  /* Check to see if the queue can take another item */
  if(queue->size + 1 > queue->max_size) {
    pthread_mutex_unlock(&queue->mutex);
    return;
  }

  /* Create the book order link */
  bo_link toadd = link_init(order);
  list_addr(queue->list, toadd);
  queue->size++;

  pthread_mutex_unlock(&queue->mutex);
}

book_order dequeue(bo_queue queue) {
  /* Lock the queue */
  pthread_mutex_lock(&queue->mutex);

  /* Dequeue item from the list, if is not empty */
  if(queue->size == 0) {
    pthread_mutex_unlock(&queue->mutex);
    return NULL;
  }

  /* Get the current rear of the list */
  /* Remove the rear from the current list */
  book_order order = queue->list->rear->order;
  
  if(order != NULL) {
    list_rr(queue->list);
    queue->size -= 1;
  }

  /* Unlock the queue */
  pthread_mutex_unlock(&queue->mutex);
  
  return order;
} 

int is_empty(bo_queue queue) {
  pthread_mutex_lock(&queue->mutex);

  int ret = FALSE;
  if(queue->size == 0) {
    ret = TRUE;
  } else {
    ret = FALSE;
  } 

  pthread_mutex_unlock(&queue->mutex);
  return ret;
}

size_t get_size(bo_queue queue) {

  pthread_mutex_lock(&queue->mutex);
  size_t size = queue->size;
  pthread_mutex_unlock(&queue->mutex);

  return size;
}

int is_full(bo_queue queue) {
  if(queue == NULL) {
    return FALSE;
  }

  int ret = FALSE;

  pthread_mutex_lock(&queue->mutex);
  size_t size = queue->size;
  size_t max = queue->max_size;
  if(size == max) {
    ret = TRUE;
  } else {
    ret = FALSE;
  }
  pthread_mutex_unlock(&queue->mutex);

  return ret;
}
