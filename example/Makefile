CC = cc
CFLAGS = -Wall -Wextra -pedantic -Wmissing-prototypes

plugin.so: plugin.o _build/default/test.exe.o
	$(CC) $(CFLAGS) -o $@ -shared plugin.o _build/default/test.exe.o

_build/default/test.exe.o: test.ml
	dune build --verbose ./test.exe.o

plugin.o: plugin.c
	$(CC) $(CFLAGS) -fPIC -c plugin.c

clean:
	rm -f *.o *.so *.a
	dune clean
