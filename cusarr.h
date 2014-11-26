#ifndef CUSARR_H
#define CUSARR_H

#include "dstructs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


struct cus_array {
  customer *array;
  size_t count;
  pthread_mutex_t mutex;
  pthread_mutex_t size_mutex;
};

typedef struct cus_array* cus_array;

cus_array cus_array_init();
void cus_array_dec(cus_array arr);
int cus_array_add(cus_array arr, customer toadd);
customer* cus_get(cus_array arr, int i);
int cus_inc_count(cus_array arr);
size_t cus_get_count(cus_array arr);







#endif
