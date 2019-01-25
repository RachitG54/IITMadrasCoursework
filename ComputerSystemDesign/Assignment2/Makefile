all: prog1 prog2

prog1: prog1.c
	gcc prog1.c -o prog1

prog2: prog2.c
	gcc prog2.c -o prog2

.PHONY: clean

clean:
	rm -rf prog1 prog2 assoc*.txt *.png
