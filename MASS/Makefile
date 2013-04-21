CFLAGS=-g2

pcsw: main.o
	$(CC) -o pcsw main.o -lsvm
play: play.o
	$(CC) -o play play.o -lsvm	

clean:
	rm pcsw *.o play
