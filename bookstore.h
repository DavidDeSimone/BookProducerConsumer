#include "dstructs.h"

/* Reads in the category input file and constructs an array of category strings */
char** read_cats(char *cat_input);

/* Reads in the user database and forms an array of customer objects */
customer* read_cus(char *dbase_input);

/* Functions called for each consumer thread to start processing items in
 * their queue
 */
void process(void *arg);
