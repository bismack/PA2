CC=g++
CCFLAGS=-g -Wall -fopenmp 
OBJS= pa2.o verify.o valinput.o

all: pa2.x

pa2.x: $(OBJS) new
	$(CC) $(CCFLAGS) $(OBJS) -o pa2.x 

pa2.o: pa2.cpp 
	$(CC) $(CCFLAGS) -c pa2.cpp

valinput.o: valinput.cpp
	$(CC) $(CCFLAGS) -c valinput.cpp

verify.o: verify.cpp
	$(CC) $(CCFLAGS) -c verify.cpp

new:

clean:
	rm *.o *.x *.txt
