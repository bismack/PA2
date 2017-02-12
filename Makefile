CC=g++
CCFLAGS=-g -Wall -fopenmp 
DEPS= verify.h valinput.h choose.h
OBJS= pa2.o verify.o valinput.o choose.o

ifeq ($(txt), 1)
OPTS = -DDEBUG
endif

all: pa2.x

%.o: %.cpp $(DEPS)
	$(CC) $(OPTS) $(CCFLAGS) -c -o $@ $<

pa2.x: $(OBJS) new
	$(CC) $(CCFLAGS) $(OBJS) -o pa2.x 
	./pa2.x 0 3 6 3 a b c 

test:
	./pa2.x 0 3 4 3 a b c 
	echo "./pa2.x 0 3 4 3 a b c">>out.txt
	cat out.txt>>total.txt
	./pa2.x 1 3 5 3 a b c 
	echo "./pa2.x 1 3 5 3 a b c">>out.txt
	cat out.txt>>total.txt
	./pa2.x 2 3 3 3 a b c 
	echo "./pa2.x 2 3 3 3 a b c">>out.txt
	cat out.txt>>total.txt
	./pa2.x 3 3 4 3 a b c 
	echo "./pa2.x 3 3 4 3 a b c">>out.txt
	cat out.txt>>total.txt
	
new:

clean:
	rm *.o *.x *.txt

