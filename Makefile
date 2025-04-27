OBJS = sam.o main.o

CC = gcc
CFLAGS = -Wall -Os
LFLAGS = 

sam: $(OBJS)
	$(CC) -o sam $(OBJS) $(LFLAGS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
