CC = gcc
OUTPUT = bookstore
CFLAGS = -g -Wall
LFLAGS = -pthread -I.
OBJ = dstructs.o list.o queue.o strarr.o bookstore.o cusarr.o

all:$(OUTPUT)

$(OUTPUT):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUTPUT) $(LFLAGS)

dstructs.o: dstructs.c 
	$(CC) $(CFLAGS) -o dstructs.o -c dstructs.c $(LFLAGS)

list.o: list.c 
	$(CC) $(CFLAGS) -o list.o -c list.c $(LFLAGS)

queue.o: queue.c
	$(CC) $(CFLAGS) -o queue.o -c queue.c $(LFLAGS)

strarr.o: strarr.c
	$(CC) $(CFLAGS) -o strarr.o -c strarr.c $(LFLAGS)

bookstore.o: bookstore.c
	$(CC) $(CFLAGS) -o bookstore.o -c bookstore.c $(LFLAGS)

cusarr.o: cusarr.c
	$(CC) $(CFLAGS) -o cusarr.o -c cusarr.c $(LFLAGS)


clean:
	rm ./*.o ./bookstore
