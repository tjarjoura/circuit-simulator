CFLAGS = -Wall -Werror -g

all: main.c tokens.c
	gcc $(CFLAGS) -o circuits main.c tokens.c 
clean:
	rm -f *.o circuits
