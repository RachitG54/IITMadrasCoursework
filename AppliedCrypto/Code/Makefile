all: cipher

cipher: cipher.o
	gcc cipher.o -o cipher -lm

cipher.o: cipher.c
	gcc -c cipher.c cipher.h

clean:
	rm -rf *.o
	rm -rf *.h.gch
	rm -rf cipher
