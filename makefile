CC=gcc
CFLAGS=-Wall -g -ansi -pedantic
EJS= prueba

all: $(EJS)

prueba: prueba.o
	$(CC) $(CFLAGS) -o prueba prueba.o transforma.o afnd.o pack.o

prueba.o: prueba.c transforma.o afnd.o pack.o
	$(CC) $(CFLAGS) -c prueba.c

afnd.o: afnd.c afnd.h transforma.o pack.o
	$(CC) -g -ansi  -c afnd.c 

transforma.o: transforma.c transforma.h pack.o
	$(CC) $(CFLAGS) -c transforma.c

pack.o: pack.c pack.h
	$(CC) $(CFLAGS) -c pack.c

clean:
	rm -f *.o $(EJS) *.png