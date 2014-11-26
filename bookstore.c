#include "dstructs.h"
#include <stdio.h>



int main(int argc, char **args) {
  if(argc != 4) {
    printf("Error, incorrect number of file arguments!\n");
    return -1;
  }

  char *dbase_input = args[1];
  char *book_input = args[2];
  char *cat_input = args[3];

  /* Read the catagory file and create the catagory struct */
  char **q_enum = read_cats(cat_input);

  /* Process the customer file and create the customer struct */
  customer *cus_arr = read_cus(dbase_input);

  /* Spawn the Producer */

  /* Spawn the consumers */

  /* For each consumer, spawn a thread and start processing data in the queue */


  /* Begin to read over the assignment file. If possible, add order to the correct queue
   * and notify the corresponding consumer it has data. If that queue is full, hold until you can add it   
   */

  /* When the producer is notified of a completed order, adds the order to the completed struct and has it get written
   */ 

 
  return 0;
}
