#include "dstructs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Possible TODO, check if id is unique?
book_order bo_init(char *title, int id, char *category, double cost) {
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

  /* Set the cost of the order */
  bo->cost = cost;

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

customer cu_init(char *name, int id, double c_limit) {
  customer cu = malloc(sizeof(struct customer));

  /* Copy the name  */
  char *name_cpy = malloc((strlen(name) + 1) * sizeof(char));
  strcpy(name_cpy, name);
  cu->name = name_cpy;

  /* Copy the id and credit limit */
  cu->id = id;
  cu->c_limit = c_limit;

  /* Initalize the amount spent to 0*/
  cu->spent = 0;

  /* Signifies if the customer has an open order */
  cu->open_order = 1;

  /* Initalize compelted order array */
  cu->comp_orders = str_array_init();
  cu->rej_orders = str_array_init();

  /* Create the mutex for this object */
  pthread_mutex_init(&cu->mutex, 0);


  return cu;
}

void cu_dec(customer cus) {
  if(cus == NULL) {
    return;
  }

  pthread_mutex_destroy(&cus->mutex);
  free(cus->name);
  free(cus);
}

producer pro_init(str_array array) {
  producer pro = malloc(sizeof(struct producer));
  int i;
  
  /* Initalize all fields to NULL */
  pro->q_enum = array;
  pro->num_consumers = array->count;
  pro->book_input = NULL;
  pro->consumers = NULL;


  /* Create an empty queue for each category */
  bo_queue *queues = malloc(sizeof(bo_queue) * pro->num_consumers);

  for(i = 0; i < pro->num_consumers; i++) {
    queues[i] = bo_queue_init(DEFAULT_LIMIT);
  }

  pro->queues = queues;

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

consumer con_init(str_array cats, char *category, bo_queue queue, cus_array customers) {
  consumer con = malloc(sizeof(struct consumer));
  int cat_id = -1;

  con->isopen = 1;

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
 
  /* Set the customer list */
  con->customers = customers;

  /* Initalize the conditional variable */
  pthread_cond_init(&con->data_available, 0);
  pthread_mutex_init(&con->mutex, 0);

  return con;
}

void con_dec(consumer con) {
  if(con == NULL) {
    return;
  }

  free(con->category);
  free(con);
}

double cu_get_diff(customer cus) {
  double ret = 0;

  if(cus == NULL) {
    return 0;
  }

  pthread_mutex_lock(&cus->mutex);
  ret = cus->c_limit - cus->spent;
  pthread_mutex_unlock(&cus->mutex);

  return ret;
}

int cu_get_open(customer cus) {
  int ret = FALSE;

  if(cus == NULL) {
    return FALSE;
  }

  pthread_mutex_lock(&cus->mutex);
  ret = cus->open_order;
  pthread_mutex_unlock(&cus->mutex);

  return ret;
}

void cu_set_open(customer cus, int set) {
  if(cus == NULL) {
    return;
  }

  printf("OPEN STATUS SET to %d\n", set);

  pthread_mutex_lock(&cus->mutex);
  cus->open_order = set;
  pthread_mutex_unlock(&cus->mutex);
}


double cu_get_climit(customer cus) {
  double ret = 0;
  
  if(cus == NULL) {
    return 0;
  }

  pthread_mutex_lock(&cus->mutex);
  ret = cus->c_limit;				
  pthread_mutex_unlock(&cus->mutex);

  return ret;
}

double cu_get_spent(customer cus) {
  double ret = 0;
  
  if(cus == NULL) {
    return 0;
  }

  pthread_mutex_lock(&cus->mutex);
  ret = cus->spent;
  pthread_mutex_unlock(&cus->mutex);

  return ret;
}



int get_catid(str_array cats, char *category) {
  int i;

  if(cats == NULL) {
    return CAT_NOT_FOUND;
  }

  for(i = 0; i < cats->count; i++) {
    if(strcmp(category, str_array_get(cats, i)) == 0) {
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

  /* Check to see if book_db is a file findable */
  FILE *f = fopen(book_db, "r");
  if(f == NULL) {
    printf("Book Database not found!\n");
    return -2;
  }
  fclose(f);

  char *cpy = malloc((strlen(book_db) + 1) * sizeof(char));
  strcpy(cpy, book_db);

  prod->book_input = cpy;

  return 0;
}

int spend(customer cu, double charge) {
  if(cu == NULL) {
    return -1;
  }

  int ret = 0;
  pthread_mutex_lock(&cu->mutex);
  if(charge + cu->spent <= cu->c_limit) {
    cu->spent += charge;
    ret = CHARGE_SUC;
  } else {
    ret = OVERSPEND;
  }
  pthread_mutex_unlock(&cu->mutex);
  
  return ret;

}

int bo_eq(book_order o1, book_order o2) {
  if(o1 == NULL || o2 == NULL) {
    return FALSE;
  }

  if(strcmp(o1->title, o2->title) == 0 && o1->id == o2->id) {
    return TRUE;
  }

  return FALSE;
}
