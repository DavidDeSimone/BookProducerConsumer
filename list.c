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

int list_addr(bo_list list, bo_link link) {
  if(list == NULL) {
    return LIST_NULL;
  }

  /* If the list is empty */
  if(list->rear == NULL) {
    list->rear = link;
    link->next = link;

    return 0;
  }

  /* Else the list is non-empty */
  bo_link tmp = list->rear->next;
  list->rear->next = link;
  link->next = tmp;

  return 0;
}

int list_rr(bo_list list) {
  if(list == NULL) {
    return LIST_NULL;
  }

  if(list->rear == NULL) {
    return ITEM_NOT_FOUND;
  }

  if(list->rear->next == NULL) {
    return MALFORMED_LIST;
  }

  /* If the item is one entry, set the list to null and return */
  if(link_eq(list->rear, list->rear->next)) {
    link_dec(list->rear);
    list->rear = NULL;
    
    return 0;
  }

  /* Remove the selected node from the list */
  bo_link prev = NULL;
  bo_link curr = list->rear;
  
  int insert = FALSE;
  
  /* Iterate over the list */
  while(!insert) {
    /* If previous is not null, and we have the rear pointer */
    if(link_eq(curr, list-rear) && prev != NULL) {
      prev->next = curr->next;
      link_dec(curr);
      
      insert = TRUE;
    } else {
      /* Else iterate to the next item in the list */
      prev = curr;
      curr = curr->next;
    }
  }

  return 0;
}

int link_eq(bo_link l1, bo_link l2) {
  if(l1 == l2) {
    return TRUE;
  } else {
    return FALSE;
  }
}
