CC=gcc

PROGS=client.o server.o

.DUMMY: all
all: $(PROGS)

client.o: wrapper.c common.c
	$(CC) -std=gnu11 -D_GNU_SOURCE -g -Wall -Wextra -shared -fPIC -o client.o wrapper.c common.c -lsasl2 -ldl

server.o: wrapper_s.c common.c
	$(CC) -std=gnu11 -D_GNU_SOURCE -g -Wall -Wextra -shared -fPIC -o server.o wrapper_s.c common.c -lsasl2 -ldl

.DUMMY: clean
clean:
	$(RM) $(PROGS)
