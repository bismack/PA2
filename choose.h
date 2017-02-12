#ifndef CHOOSE_H
#define CHOOSE_H

using namespace std;

extern int counter;
extern int occurOfC0, occurOfC1, occurOfC2;
extern string alphabet;

class INPUT {
	public:
	int F;
	int N;
   	int L;
   	char c0, c1, c2;
};

char chooseChar(int ranA, int ranC, INPUT inSt);

#endif

