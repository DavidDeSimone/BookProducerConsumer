#ifndef STRARR_H
#define STRARR_H

#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct str_array {
  char **strs;
  size_t count;
  pthread_mutex_t mutex;
  pthread_mutex_t size_mutex;
};

typedef struct str_array* str_array;

str_array str_array_init();
void str_array_dec(str_array to_dec);
void strs_dec(char **strs, size_t count);
int str_array_add(str_array arr, char *str);
size_t str_array_get_size(str_array arr);
int str_array_inc_size(str_array arr);


#endif
