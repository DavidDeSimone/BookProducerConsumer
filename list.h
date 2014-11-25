#ifndef LIST_H
#define LIST_H

#include "dstructs.h"
#include "queue.h"

struct bo_link {
  book_order order;
  struct bo_link *next;
};

typedef struct bo_link* bo_link;

struct bo_list {
  bo_link rear;
};

typedef struct bo_list* bo_list;

bo_link link_init(book_order order);
bo_list list_init();

void list_dec(bo_list list);
void link_dec(bo_link link);

/* Adds a passed link to the front of the circular list */
int list_add(bo_list list, bo_link link);

/* Removes the first instance of the passed link encountered. Returns -1 if not found */
int list_rm(bo_list list, bo_link link);

#endif
