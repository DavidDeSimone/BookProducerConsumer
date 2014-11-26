#ifndef DSTRUCTS_H
#define DSTRUCTS_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

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
  char **q_enum;
  bo_queue *queues;
  size_t num_consumers;
};

/* Consumers will process orders added to their repspective category queues
 * When they process an order, they will notify the producer, and he will add another 
 * order to process to their queue. 
 */
struct consumer {
  char *category;
  int cat_id;
};

typedef struct consumer* consumer;

/* Struct constructors */
book_order bo_init(char *title, int id, char *category);
customer cu_init(char *name, int id, int c_limit);
producer pro_init();
consumer con_init(char **cats, char *category);

/* Struct deconstructors */
void bo_dec(book_order order);
void cu_dec(customer cus);
void pro_dec(producer pro);
void con_dec(consumer con);

/* Add a category queue to a producer object */
int pro_add_queue(producer pro, bo_queue queue);

/* Adds a category name to the category enumeration */
int add_category(char **cats, char *cat_name);

/* Searches for the category passed at category in the string array
 * If the string is found, returns the categories index
 * else it returns -1
 */
int get_catid(char **cats, char *category);

#endif
