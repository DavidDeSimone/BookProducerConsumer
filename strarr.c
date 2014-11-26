#include "strarr.h"

/* Thread safe implemenation of a 2D string array */
str_array str_array_init() {
  str_array sa = malloc(sizeof(struct str_array));
  sa->strs = NULL;
  sa->count = 0;
  pthread_mutex_init(&sa->mutex, 0);
  pthread_mutex_init(&sa->size_mutex, 0);

  return sa;
}

void str_array_dec(str_array to_dec) {
  if(to_dec == NULL) {
    return;
  }

  strs_dec(to_dec->strs, to_dec->count);
  pthread_mutex_destroy(&to_dec->mutex);
  pthread_mutex_destroy(&to_dec->size_mutex);

  free(to_dec);
}

void strs_dec(char **strs, size_t count) {
  int i;
  for(i = 0; i < count; i++) {
    free(strs[i]);
  }
}

char* str_array_get(str_array arr, int i) {
  if(arr == NULL) {
    return NULL;
  }

  size_t count = str_array_get_size(arr);
  if(i >= count || i < 0) {
    return NULL;
  }

  char* to_ret = NULL;

  pthread_mutex_lock(&arr->mutex);
  to_ret = arr->strs[i];
  pthread_mutex_unlock(&arr->mutex);

  return to_ret;
}


int str_array_add(str_array arr, char *str) {
  int i = 0;

  if(str == NULL) {
    return -1;
  }

  pthread_mutex_lock(&arr->mutex);
  /* Make a copy of the string */
  char *cpy = malloc((strlen(str) + 1) * sizeof(char));
  strcpy(cpy, str);

  /* If the list is empty */
  if(str_array_get_size(arr) == 0) {
    /* Inialize it with the first entry */
    char **strs = malloc(sizeof(char *));
    strs[0] = cpy;
    str_array_inc_size(arr);
    
    pthread_mutex_unlock(&arr->mutex);
    return 0;
  }
  
  /* Else copy over the current list */
  size_t count = str_array_get_size(arr);

  char **strs = malloc((count + 1) * sizeof(char *));

  for(i = 0; i < count; i++) {
    strs[i] = arr->strs[i];
  }

  /* Append an entry */
  strs[count] = cpy;

  /* Delete the old list */
  //strs_dec(arr->strs, arr->count);

  /* Update list in struct */
  arr->strs = strs;
  str_array_inc_size(arr);

  pthread_mutex_unlock(&arr->mutex);
  return 0;

}

size_t str_array_get_size(str_array arr) {
  if(arr == NULL) {
    return 0;
  }

  size_t count = 0;
  pthread_mutex_lock(&arr->size_mutex);
  count = arr->count;
  pthread_mutex_unlock(&arr->size_mutex);

  return count;
}

int str_array_inc_size(str_array arr) {
  if(arr == NULL) {
    return -1;
  }

  pthread_mutex_lock(&arr->size_mutex);
  arr->count++;
  pthread_mutex_unlock(&arr->size_mutex);

  return 0;
}
