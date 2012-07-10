CFLAGS=-std=gnu99 -Wall -lpthread -lgcc_s -g 

all:webWaiter

webWaiter:startup.o request.o response.o debug.o

clean:
	rm -rf webWaiter *.o
