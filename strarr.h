#include <string.h>
#include <pthread.h>

struct str_array {
  char **strs;
  size_t count;
};

typedef strcut str_array* str_array;

str_array str_array_init();
void str_array_dec(str_array to_dec);
void strs_dec(char **strs, size_t count);
int str_array_add(str_array arr, char *str, pthread_mutex_t mutex);

