CC=g++
CCFLAGS=-g -Wall -fopenmp 
OBJS= pa2.o

all: pa2.x 

pa2.x: $(OBJS) new
	$(CC) $(CCFLAGS) $(OBJS) -o pa2.x 

pa2.o: pa2.cpp 
	$(CC) $(CCFLAGS) -c pa2.cpp -fopenmp

new:

clean:
	rm *.o *.x
