#include "cusarr.h"

cus_array cus_array_init() {
  cus_array ret = malloc(sizeof(struct cus_array));
  ret->array = NULL;
  ret->count = 0;
  pthread_mutex_init(&ret->mutex, 0);
  pthread_mutex_init(&ret->size_mutex, 0);

  return ret;
}

void cus_array_dec(cus_array arr) {
  //TODO
}

int cus_array_add(cus_array arr, customer toadd) {
  if(toadd == NULL) {
    return -1;
  }

  /* Lock the function */
  pthread_mutex_lock(&arr->mutex);
  
  /* If the array is unitialized */
  if(arr->array == NULL) {
    arr->array = malloc(sizeof(customer *));
    arr->array = toadd;
    cus_inc_count(arr);

    pthread_mutex_unlock(&arr->mutex);
    return 0;
  }

  /* Else copy the current array and add one */
  size_t size = cus_get_count(arr);
  int i;
  customer *cpy = malloc((size + 1) * sizeof(customer *));

  for(i = 0; i < size; i++) {
    cpy[i] = arr->array[i];
  }

  cpy[size] = to_add;
  cus_inc_count(arr);

  pthread_mutex_unlock(&arr->mutex);
  
  return 0;
}

size_t cus_get_count(cus_array arr) {
  size_t toret;
  
  if(arr == NULL) {
    return 0;
  }

  pthread_mutex_lock(&arr->size_mutex);
  
  toret = arr->count;

  pthread_mutex_unlock(&arr->size_mutex);

  return toret;
}

int cus_inc_count(cus_array arr) {
  if(arr == NULL) {
    return -1;
  }

  pthread_mutex_lock(&arr->size_mutex);
  arr->count++;
  pthread_mutex_unlock(&arr->size_mutex);

  return 0;
}

customer* cus_get(cus_array arr, int i) {
  if(arr == NULL) {
    return NULL;
  }

  size_t count = cus_get_count(arr);

  if(i < 0 || i >= count) {
    return NULL;
  }

  pthread_mutex_lock(&arr->mutex);
  customer to_ret = arr->array[i];
  pthread_mutex_unlock(&arr->mutex);

  return to_ret;
}