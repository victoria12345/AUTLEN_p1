CC=gcc
CFLAGS=-Wall -g -ansi -pedantic
EJS= prueba prueba_afnd

all: $(EJS)

prueba_afnd: prueba_afnd.o
	$(CC) $(CFLAGS) -o prueba_afnd prueba_afnd.o transforma.o afnd.o pack.o

prueba_afnd.o: prueba_afnd.c transforma.o afnd.o pack.o
	$(CC) $(CFLAGS) -c prueba_afnd.c

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

img: 
	dot -Tpng af11.dot > af11.png
	dot -Tpng afd.dot > afd.png

clean:
	rm -f *.o $(EJS) *.png *.dot