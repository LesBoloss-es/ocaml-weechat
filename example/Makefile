.POSIX:

CC = cc
CFLAGS = -Wall -Wextra -pedantic
DUNE = dune

plugin.so: plugin.ml
	$(DUNE) build ./plugin.exe.o
	$(CC) $(CFLAGS) -o $@ -shared \
		$$($(DUNE) exec ocamlfind query weechat)/plugin/plugin_boilerplate.o \
		../_build/default/example/plugin.exe.o

description.o: description.c
	$(CC) -fPIC -c description.c

clean:
	rm -f *.o *.so
