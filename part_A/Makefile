all: maths_prog

add.o: add.c add.h
	gcc -Wall -Wextra -pedantic -std=c11 -fPIC -c add.c

subtract.o: subtract.c subtract.h
	gcc -Wall -Wextra -pedantic -std=c11 -fPIC -c subtract.c

maths_prog.o: maths_prog.c
	gcc -Wall -Wextra -pedantic -std=c11 -c maths_prog.c

libmaths.so: add.o subtract.o
	gcc -Wall -Wextra -pedantic -std=c11 -shared -o libmaths.so add.o subtract.o

maths_prog: libmaths.so maths_prog.o
	gcc -Wall -Wextra -pedantic -std=c11 -o maths_prog maths_prog.o -L`pwd` -lmaths

clean:
	rm -rf libmaths.so maths_prog.o maths_prog add.o subtract.o
