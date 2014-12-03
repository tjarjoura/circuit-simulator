CFLAGS = -Wall -Werror -g

all: evaluate.c inputs.c label.c main.c parse.c tokens.c
	gcc $(CFLAGS) -o circuits evaluate.c inputs.c label.c main.c parse.c tokens.c
clean:
	rm -f *.o circuits
