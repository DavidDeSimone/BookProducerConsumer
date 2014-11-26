#ifndef DSTRUCTS_H
#define DSTRUCTS_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "strarr.h"
#include "queue.h"
#include <string.h>

#define CAT_NOT_FOUND -1


/* Struct representing a book order.
 */
struct book_order {
  char *title;
  int id;
  char *category;
};

typedef struct book_order* book_order;

struct customer {
  char *name;
  int id;
  int c_limit;
};

typedef struct customer* customer;

/* Struct representing the producer in the Producer/Consumer model
 * Producer will initalize the master list of category queues
 * After this, he will add orders to appropriate queues when each queue has an opening
 */
struct producer {
  str_array q_enum;
  struct bo_queue **queues;
  char *book_input;

  pthread_t *tids;
  size_t num_consumers;
};

typedef struct producer* producer;

/* Consumers will process orders added to their repspective category queues
 * When they process an order, they will notify the producer, and he will add another 
 * order to process to their queue. 
 */
struct consumer {
  char *category;
  struct bo_queue *queue;
  int cat_id;

  str_array comp_orders;
  str_array rej_orders;
};

typedef struct consumer* consumer;

/* Struct constructors */
book_order bo_init(char *title, int id, char *category);
customer cu_init(char *name, int id, int c_limit);
producer pro_init(str_array q_enum);
consumer con_init(str_array cats, char *category, struct bo_queue *queue);

/* Struct deconstructors */
void bo_dec(book_order order);
void cu_dec(customer cus);
void pro_dec(producer pro);
void con_dec(consumer con);

/* Add a category queue to a producer object */
int pro_add_queue(producer pro, struct bo_queue *queue);

/* Adds a category name to the category enumeration */
int add_category(str_array cats, char *cat_name);

/* Searches for the category passed at category in the string array
 * If the string is found, returns the categories index
 * else it returns -1
 */
int get_catid(str_array cats, char *category);

/* Sets the book database for the given producer */
int set_bookdb(producer prod, char *book_db);

#endif
