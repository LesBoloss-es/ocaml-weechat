CC = cc
CFLAGS = -Wall -Wextra -pedantic -Wmissing-prototypes \
				 -I$(shell ocamlc -where)

plugin.so: plugin.o src
	dune build src/plugin.exe.o
	$(CC) $(CFLAGS) -o $@ -shared plugin.o _build/default/src/plugin.exe.o

plugin.o: plugin.c
	$(CC) $(CFLAGS) -fPIC -c plugin.c

clean:
	rm -f *.o *.so *.a
	dune clean
