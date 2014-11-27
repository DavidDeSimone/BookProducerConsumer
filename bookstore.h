#ifndef BOOK_H
#define BOOK_H
#define _GNU_SOURCE

#include "dstructs.h"
#include "strarr.h"
#include "cusarr.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Reads in the category input file and constructs an array of category strings */
str_array read_cats(char *cat_input);

/* Reads in the user database and forms an array of customer objects */
cus_array read_cus(char *dbase_input);

/* Functions called for each consumer thread to start processing items in
 * their queue
 */
void* process(void *arg);

/* Reads the book order database line by line and adds
 * order to appropriate ordered queue 
 */

/* Given a producer with a non-null category list, spawns a consumer object for each category */
consumer* spawn_consumers(producer prod, cus_array customers);

/* Reads the book list associated with the passed producer object
 * Parses input file line by line, and adds each order to corresponding
 * consumer queue. Notifies consumer on this action
 */
void* read_data(void *producer);

/* Writes order output for each customer in the customer array. See 
 * readme.pdf for sample output
 */
void write_output(cus_array customers);

#endif
