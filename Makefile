CC = gcc
CFLAGS = -Wall -g

build: search_index

search_index: tema2.c main.c
	$(CC) $(CFLAGS) $^ -o $@

run:
	./search_index

.PHONY: clean
clean:
	rm -f *.o *~ search_index

pack:
	zip 314CB_MIRAUTA_BOGDAN-CONSTANTIN_tema2.zip main.c tema2.c tema2.h README Makefile