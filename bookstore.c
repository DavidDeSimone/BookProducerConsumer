#include "bookstore.h"

#define DEBUG 1

str_array read_cats(char *cat_input) {
  FILE *input;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  str_array ret = str_array_init();

  /* Open the file */
  input = fopen(cat_input, "r");

  if(input == NULL) {
    printf("Error, file %s not found!\n", cat_input);
    return NULL;
  }
  
  /* Read the file line by line */
  while((read = getline(&line, &len, input)) != -1) {
    /* Strip the string of newlines (TODO careful on memory here) */
    char *pos;
    if((pos=strchr(line, '\n')) != NULL) {
      *pos = '\0';
    }

    #ifdef DEBUG
       printf("Line: %s\n", line);
    #endif

    /* Add the category to the return list, check for errors */
    int i = 0;
    if((i = str_array_add(ret, line)) != 0) {
      printf("Error adding string %s\n", line);
    }
  }

  return ret;
}

cus_array read_cus(char *dbase_input) {
  FILE *input;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  cus_array ret = cus_array_init();

  input = fopen(dbase_input, "r");

  if(input == NULL) {
    printf("Error reading database file!\n");
    return NULL;
  }

  /* Read the file line by line */
  while((read = getline(&line, &len, input)) != -1) {
    /* Strip the newlines */
    char *pos;
    if((pos=strchr(line, '\n')) != NULL) {
      *pos = '\0';
    }

  
    #ifdef DEBUG
    printf("Line: %s\n", line);
    #endif

    /* Create the customer object by tokenizing */
    char *token = NULL;
    token = strtok(line, "|");

    char *name = NULL;
    char *id_s = NULL;
    char *c_limit_s = NULL;

    int id = -1;
    int c_limit = -1;

    name = token;
    id_s = strtok(NULL, "|");
    c_limit_s = strtok(NULL, "|");

    if(name == NULL || id_s == NULL || c_limit_s == NULL) {
      printf("Malformed input file!\n");
      return NULL;
    }

    id = atoi(id_s);
    c_limit = atof(c_limit_s);

    /* Create the customer object */
    customer to_add = cu_init(name, id, c_limit);

    customer dummy = NULL;
    /* If the customer is already in the database, reject */
    if((dummy = cus_get_byid(ret, to_add->id)) != NULL) {
      printf("Customer with duplicate ID added, rejecting!\n");
      return NULL;
    }

    #ifdef DEBUG
    printf("Added customer %s %d %d\n", name, id, c_limit);
    #endif


    /* Add it to the list */
    int i = 0;
    if((i = cus_array_add(ret, to_add)) != 0) {
      printf("Error adding customer to list\n");
      return NULL;
    }

  }


  return ret;
}

consumer* spawn_consumers(producer prod, cus_array customers) {
  if(prod == NULL) {
    return NULL;
  }

  /* Allocate memory for consumer array */
  size_t size = prod->num_consumers;
  consumer* con_array = malloc(size * sizeof(consumer));
  int i;

  for(i = 0; i < size; i++) {
    /* Create a consumer struct */
    str_array cats = str_array_init();
    char *cat = str_array_get(prod->q_enum, i);
    bo_queue queue = prod->queues[i];


    #ifdef DEBUG
    printf("Consumer spanwed for category %s\n", cat);
    #endif


    consumer con = con_init(cats, cat, queue, customers);
    con_array[i] = con;
  }
  
  return con_array;
}


void* process(void *cons) { 
  consumer con = (consumer)cons;

  while(con->isopen) {

    pthread_mutex_lock(&con->mutex);

    //printf("Thread %u awaiting data from consumer\n", (unsigned int)pthread_self());
    /* Wait for the consumer to alert you of an addition */
    //pthread_cond_wait(&con->data_available, &con->mutex);
    
    /* If the producer has shut down this consumer, break */
    //if(!con->isopen) break;

    //printf("Thread %u recv data, processing queue members\n", (unsigned int)pthread_self());

    while(!is_empty(con->queue)) {
      /* Take an object out from the queue */
      book_order order = dequeue(con->queue);

      #ifdef DEBUG
      printf("Order Dequeued, Title: %s, ID: %d\n", order->title, order->id);
      #endif

      if(order != NULL) {
	customer sucker = cus_get_byid(con->customers, order->id);
	if(sucker == NULL) {
	  printf("Error, sucker not found!, %d\n", order->id);
	  return NULL;
	}
	
	/* Process the object */
	int result = spend(sucker, order->cost);
	
	printf("Processed Order for %s by %u\n", sucker->name, (unsigned int)pthread_self());

	/* Update customer record for this transaction */
	/* If a charge is successful, print a string formatted that describes its title, cost, and amount spent so far */
	/* Else, print the title and cost of the rejected order */
	if(result == CHARGE_SUC) {
	  double diff = cu_get_diff(sucker); 
	  
	  char zz[500];
	  snprintf(zz, 500, "%s|%g|%g\n", order->title, order->cost, diff);

	  char *ptr = (char*)&zz;
	  str_array_add(sucker->comp_orders, ptr);

	} else {
	  char zz[500];
	  snprintf(zz, 500, "%s|%g\n", order->title, order->cost);

	  char *ptr = (char*)&zz;
	  str_array_add(sucker->rej_orders, ptr);

	}
       
	cu_set_open(sucker, FALSE);
	/* Go back to waiting for the producer*/
	bo_dec(order);
    
      }
    }
    pthread_mutex_unlock(&con->mutex);
  }

  return NULL; 
}

void* read_data(void *produ) {
  producer prod = (producer)produ;
  FILE *input;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  
  /* Open the file */
  if(prod->book_input == NULL) {
    printf("Error, producer not properly instansiated. Please specify order file\n");
    return NULL;
  }

  input = fopen(prod->book_input, "r");

  if(input == NULL) {
    printf("Error, file not found!\n");
    return NULL;
  }

  /* For each line of the file */
  while((read = getline(&line, &len, input)) != -1) {
    /* Strip newline */
    char *pos;
    if((pos=strchr(line, '\n')) != NULL) {
      *pos = '\0';
    }

    /* Read the line and create the book order object */
    char *title = NULL;
    char *id_s = NULL;
    char *cost_s = NULL;
    char *category = NULL;

    int id = -1;
    double cost = -1;


    title = strtok(line, "|");
    cost_s = strtok(NULL, "|");
    id_s = strtok(NULL, "|");
    category = strtok(NULL, "|");

    id = atoi(id_s);
    cost = atof(cost_s);

    if(title == NULL || id_s == NULL || cost_s == NULL || category == NULL) {
      printf("Error in processing book order!\n");
      return NULL;
    }

    book_order order = bo_init(title, id, category, cost);

    #ifdef DEBUG
    printf("Created Order %s %d %s\n", title, id, category);
    #endif

    /* Find the appropriate consumer object */
    int i;
    consumer noted = NULL;
    if((i = get_catid(prod->q_enum, category)) == -1) {
      printf("Error, requested category not found!\n");
      return NULL;
    }
    
    noted = prod->consumers[i];

    /* If the consumers queue is not full, add to the queue and alert the consumer */

    #ifdef DEBUG
    //WARNING BAD
    //ADD CLOSED CUSTOMERS TO QUEUE AND PROCESS FROM THAT IF NON EMPTY
    customer cus = cus_get_byid(noted->customers, id);
    if(cu_get_open(cus)) {
      while(TRUE) {
	sleep(1);
	if(!cu_get_open(cus)) break;
      }
    }

    #endif
    
    bo_queue queue = noted->queue;
    if(!is_full(queue)) {
      enqueue(queue, order);
      cu_set_open(cus, TRUE);
      //pthread_cond_signal(&noted->data_available);
      printf("Sending Signal...\n");
    } else {
      /* If the consumers queue IS full, wait some time and try to add it again */
      while(TRUE) {
	sleep(1);
	if(!is_full(queue)) {
	  enqueue(queue, order);
	  pthread_cond_signal(&noted->data_available);
	  break;
	}
      }
    }
   
  }
 

  /* Wait for the consumers to consume */
  /* When the entire order list is processed, do the following 
   * For Each Consumer:
   *    Signal "data available". This is to trigger threads blocked on this call
   * Set the consumers isopen flag to false
   * Join on the consumer thread
   *
   * Each consumer will run until its queue is empty and its isopen flag is set to false
   * When all the consumers have joined, add up the total amount spent, and process the customer record array. 
   * All consumers have the same master customer record array (which is a thread safe vector implementation) so selecting the pointer from the first consumer will be fine.
   */
  int j;
  double spent = 0;
  customer cus = NULL;
  for(j = 0; j < prod->num_consumers; j++) {
    prod->consumers[j]->isopen = 0;
    pthread_cond_signal(&prod->consumers[j]->data_available);
    printf("Waiting on thread %u to exit\n", (unsigned int)prod->tids[j]);
    pthread_join(prod->tids[j], 0);
  }

  for(j = 0; j < prod->consumers[0]->customers->count; j++) {
    cus = cus_get(prod->consumers[0]->customers, j);
    spent += cu_get_spent(cus);
  }

  printf("Total amount earned for this round, %g\n", spent);
  write_output(prod->consumers[0]->customers);

  return NULL; 
}

void write_output(cus_array customers) {
  FILE *output;
  output = fopen("output.txt", "w");

  if(output == NULL) {
    printf("Failed to write output file!\n");
    return;
  }


  int i;
  for(i = 0; i < customers->count; i++) {
    fprintf(output, "=== BEGIN CUSTOMER INFO ===\n");
    customer cus = cus_get(customers, i);

    fprintf(output, "### BALANCE ###\n");
    fprintf(output, "Customer name: %s\n", cus->name);
    fprintf(output, "Customer id: %d\n", cus->id);
    double diff = cu_get_diff(cus);
    fprintf(output, "Remaining credit balance after all purchases (a dollar amount): %g\n", diff);

    fprintf(output, "### SUCCESSFUL ORDERS ###\n");
    
    int j;
    for(j = 0; j < cus->comp_orders->count; j++) {
      fprintf(output, "%s", str_array_get(cus->comp_orders, j));
    }
    
    fprintf(output, "### REJECTED ORDERS ###\n");
    int k;
    for(k = 0; k < cus->rej_orders->count; k++) {
      fprintf(output, "%s", str_array_get(cus->rej_orders, k));
    }

    fprintf(output, "=== END CUSTOMER INFO ===\n");
    fprintf(output, "\n");
  }

  fclose(output);
}

int main(int argc, char **args) {
  if(argc != 4) {
    printf("Error, incorrect number of file arguments!\n");
    return -1;
  }

  char *dbase_input = args[1];
  char *book_input = args[2];
  char *cat_input = args[3];
  pthread_t prod_th;

  /* Read the catagory file and create the catagory struct */
  str_array q_enum;
  if((q_enum = read_cats(cat_input)) == NULL) {
    printf("Error processing category input.\n");
    return -1;
  }
  
  /* Process the customer file and create the customer struct */
  cus_array cus_arr = NULL;
  if((cus_arr = read_cus(dbase_input)) == NULL) {
    printf("Error processing customer database.\n");
    return -2;
  }

  /* Spawn the Producer */
  producer prod = pro_init(q_enum);
  if((set_bookdb(prod, book_input)) != 0) {
    printf("Error setting book DataBase\n");
    return -3;
  }


  /* Spawn the consumers */
  consumer* consumers = spawn_consumers(prod, cus_arr);

  prod->consumers = consumers;

  /* For each consumer, spawn a thread and start processing data in the queue */
  int i;
  for(i = 0; i < prod->num_consumers; i++) {
    pthread_create(&prod->tids[i], NULL, process, consumers[i]);
  }

  /* Begin to read over the assignment file. If possible, add order to the correct queue
   * and notify the corresponding consumer it has data. If that queue is full, hold until you can add it   
   */
  pthread_create(&prod_th, NULL, read_data, prod);
  

  pthread_exit(0);
}

