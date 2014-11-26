#include "bookstore.h"

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
    c_limit = atoi(c_limit_s);

    /* Create the customer object */
    customer to_add = cu_init(name, id, c_limit);

    /* Add it to the list */
    int i = 0;
    if((i = cus_array_add(ret, to_add)) != 0) {
      printf("Error adding customer to list\n");
      return NULL;
    }

  }


  return ret;
}


consumer* spawn_consumers(producer prod) {

  return NULL;
}


void* process(void *consumer) { 
  return NULL; 
}

void* read_data(void *producer) { 
  return NULL; 
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
  cus_array cus_arr;
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
  consumer* consumers = spawn_consumers(prod);

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
  return 0;
}

