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
  if(cu_dec == NULL) {
    return;
  }
  
  free(cu->name);
  free(cu);
}

producer pro_init() {
  producer pro = malloc(sizeof(struct producer));

  /* Initalize all fields to NULL */
  pro->q_enum = NULL;
  pro->queues = NULL;
  pro->num_consumers = 0;
}

void pro_dec(producer pro) {
  int i;
  
  if(pro == NULL) {
    return;
  }

  /* Free each string in the enumeration */
  /* and the corresponding queue in the queue array*/
  for(i = 0; i < pro->num_consumers; i++) {
    free(pro->q_enum[i]);
    bo_queue_dec(pro->queues[i]);
  }

  free(pro);
}

consumer con_init(char **cats, char *category) {
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

}

void con_dec(consumer con) {
  if(con == NULL) {
    return NULL;
  }

  free(con->category);
  free(con);
}
