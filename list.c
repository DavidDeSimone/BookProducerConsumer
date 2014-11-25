#include "list.h"

//TODO, copy book order list, dont keep reference here
bo_link link_init(book_order order) { 
  bo_link link = malloc(sizeof(struct bo_link));
  link->order = order;
  link->next = NULL;

  return link;
}

bo_list list_init() {
  bo_list list = malloc(sizeof(struct bo_list));
  list->rear = NULL;

  return list;
}

void list_dec(bo_list list) {
  if(list == NULL) {
    return;
  }

  /* Iterate over the list and dec each entry */
  bo_link link = list->rear;

  while(link != NULL) {
    bo_link tmp = link->next;
    link_dec(link);

    link = tmp;
  }

  free(list);
}

void link_dec(bo_link link) {
  //Check TODO above, dont dec book order for now
  free(link);
}

int list_add(bo_list list, bo_link link) {


  return 0;
}

int list_rm(bo_list list, bo_link link) {

  return 0;
}
