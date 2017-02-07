CC=g++
CCFLAGS=-Wall
OBJS= pa1.o verify.o valinput.o

all: pa1.x 

pa1.x: $(OBJS) new
	$(CC) $(CCFLAGS) $(OBJS) -std=c++11 -o pa1.x  -pthread
	./pa1.x 0 3 6 3 a b c

pa1.o: pa1.cpp 
	$(CC) $(CCFLAGS) -std=c++11 -c pa1.cpp -pthread

valinput.o: valinput.cpp
	$(CC) $(CCFLAGS) -std=c++11 -c valinput.cpp -pthread

verify.o: verify.cpp
	$(CC) $(CCFLAGS) -std=c++11 -c verify.cpp -pthread

less: pa1_less.o verify.o valinput.o new
	$(CC) $(CCFLAGS) pa1_less.o verify.o valinput.o -std=c++11 -o pa1.x  -pthread
	./pa1.x 0 3 6 3 a b c

pa1_less.o: pa1_less.cpp
	$(CC) $(CCFLAGS) -std=c++11 -c pa1_less.cpp -pthread

new:

clean:
	rm *.o *.x out.txt
