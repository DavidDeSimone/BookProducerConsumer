#include "strarr.h"

/* Thread safe implemenation of a 2D string array */


str_array str_array_init() {
  str_array sa = malloc(sizeof(struct str_array));
  sa->strs = NULL;
  sa->count = 0;

  return sa;
}

void str_array_dec(str_array to_dec) {
  if(to_dec == NULL) {
    return;
  }

  strs_dec(to_dec->strs, to_dec->count);

  free(to_dec);
}

void strs_dec(char **strs, size_t count) {
  int i;
  for(i = 0; i < count; i++) {
    free(strs[i]);
  }
}

int str_array_add(str_array arr, char *str, pthread_mutex_t mutex) {
  int i = 0;

  if(str == NULL) {
    return -1;
  }

  pthread_mutex_lock(&mutex);
  /* Make a copy of the string */
  char *cpy = malloc((strlen(str) + 1) * sizeof(char));
  strcpy(cpy, str);

  /* If the list is empty */
  if(arr->count == 0) {
    /* Inialize it with the first entry */
    char **strs = malloc(sizeof(char *));
    strs[0] = cpy;
    arr->count = 1;
    
    pthread_mutex_unlock(&mutex);
    return 0;
  }
  
  /* Else copy over the current list */
  char **strs = malloc((arr->count + 1) * sizeof(char *));

  for(i = 0; i < arr->count; i++) {
    strs[i] = arr->strs[i];
  }

  /* Append an entry */
  strs[arr->count + 1] = cpy;

  /* Delete the old list */
  strs_dec(arr->strs, arr->count);

  /* Update list in struct */
  arr->strs = strs;
  arr->count++;

  pthread_mutex_unlock(&mutex);
  return 0;

}
