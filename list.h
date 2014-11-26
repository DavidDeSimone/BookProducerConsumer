#ifndef LIST_H
#define LIST_H

#include "dstructs.h"
#include "queue.h"

/* List Error Conditions */
#define LIST_NULL -2 /* If the list is NULL */
#define ITEM_NOT_FOUND -3 /* If an item asked is not found in the list */
#define MALFORMED_LIST -4 /* If the list is not correctly formed */

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
int list_addr(bo_list list, bo_link link);

/* Removes the current rear of the linked list */
int list_rr(bo_list list, bo_link link);

/* Checks if two links are equal. Equality is defined as refering to the same link object (pointer comparison) */
int link_eq(bo_link l1, bo_link l2);

#endif
