#include "dstructs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Possible TODO, check if id is unique?
book_order bo_init(char *title, int id, char *category) {
  if(title == NULL || category == NULL) {
    return NULL;
  }

  /* Create the book order */
  book_order bo = malloc(sizeof(struct book_order));

  /* Copy the title into struct*/
  char *title_cpy = malloc((strlen(title) + 1) * sizeof(char));
  strcpy(title_cpy, title);
  bo->title = title_cpy;

  /* Copy the category into the struct */
  char *cat_cpy = malloc((strlen(category) + 1) * sizeof(char));
  strcpy(cat_cpy, category);
  bo->category = cat_cpy;

  /* Set the ID */
  bo->id = id;

  return bo;
 
}

void bo_dec(book_order order) {
  if(order == NULL) {
    return;
  }

  free(order->title);
  free(order->category);
  free(order);
}

customer cu_init(char *name, int id, int c_limit) {
  customer cu = malloc(sizeof(struct customer));

  /* Copy the name  */
  char *name_cpy = malloc((strlen(name) + 1) * sizeof(char));
  strcpy(name_cpy, name);
  cu->name = name_cpy;

  /* Copy the id and credit limit */
  cu->id = id;
  cu->c_limit = c_limit;

  return cu;
}

void cu_dec(customer cus) {
  if(cus == NULL) {
    return;
  }
  
  free(cus->name);
  free(cus);
}

producer pro_init(str_array array) {
  producer pro = malloc(sizeof(struct producer));

  /* Initalize all fields to NULL */
  pro->q_enum = array;
  pro->queues = NULL;
  pro->num_consumers = array->count;
  pro->book_input = NULL;

  /* Initalize empty array of pthread types */
  pthread_t *tids = malloc(pro->num_consumers * sizeof(pthread_t));
  pro->tids = tids;

  return pro;
}

void pro_dec(producer pro) {
  if(pro == NULL) {
    return;
  }

  /* Free each string in the enumeration */
  /* and the corresponding queue in the queue array*/
  str_array_dec(pro->q_enum);
  
  free(pro);
}

consumer con_init(str_array cats, char *category, bo_queue queue) {
  consumer con = malloc(sizeof(struct consumer));
  int cat_id = -1;

  /* Copy the category name */
  char *cat_cpy = malloc((strlen(category) + 1) * sizeof(char));
  strcpy(cat_cpy, category);
  con->category = cat_cpy;

  /* Find the category in the given category list */
  cat_id = get_catid(cats, category);
  if(cat_id != CAT_NOT_FOUND) {
    con->cat_id = cat_id;
  }

  /* Set the queue */
  con->queue = queue;
  
  /* Initalize the string array lists */
  con->comp_orders = str_array_init();
  con->rej_orders = str_array_init();

  return con;
}

void con_dec(consumer con) {
  if(con == NULL) {
    return;
  }

  free(con->category);
  free(con);
}

int get_catid(str_array cats, char *category) {
  int i;

  if(cats == NULL) {
    return CAT_NOT_FOUND;
  }

  for(i = 0; i < cats->count; i++) {
    if(strcmp(category, cats->strs[i]) == 0) {
      return i;
    }
  }

  /* Else entry not found */
  return CAT_NOT_FOUND;
}

int set_bookdb(producer prod, char *book_db) {
  if(prod == NULL || book_db == NULL) {
    return -1;
  }

  char *cpy = malloc((strlen(book_db) + 1) * sizeof(char));
  strcpy(cpy, book_db);

  prod->book_input = cpy;

  return 0;
}

