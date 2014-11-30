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
    arr->array = malloc(sizeof(customer));
    arr->array[0] = toadd;
    cus_inc_count(arr);

    pthread_mutex_unlock(&arr->mutex);
    return 0;
  }

  /* Else copy the current array and add one */
  size_t size = cus_get_count(arr);
  int i;
  customer *cpy = malloc((size + 1) * sizeof(customer));

  for(i = 0; i < size; i++) {
    cpy[i] = arr->array[i];
  }

  cpy[size] = toadd;
  cus_inc_count(arr);

  free(arr->array);
  arr->array = cpy;
  
  pthread_mutex_unlock(&arr->mutex);
  
  return 0;
}

size_t cus_get_count(cus_array arr) {
  size_t toret = 0;
  
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
  arr->count += 1;
  pthread_mutex_unlock(&arr->size_mutex);

  return 0;
}

customer cus_get(cus_array arr, int i) {
  if(arr == NULL) {
    return NULL;
  }

  pthread_mutex_lock(&arr->mutex);
  customer to_ret = NULL;
  size_t count = cus_get_count(arr);

  if(i < 0 || i >= count) {
    return NULL;
  }

  to_ret = arr->array[i];
  pthread_mutex_unlock(&arr->mutex);

  return to_ret;
}

customer cus_get_byid(cus_array arr, int id) {
  if(arr == NULL) {
    printf("Error, null list\n");
    return NULL;
  }

  int i;
  size_t count = cus_get_count(arr);

  for(i = 0; i < count; i++) {
    customer cmp = cus_get(arr, i);
    if(cmp->id == id) {
      return cmp;
    }
  }

  return NULL;
}
