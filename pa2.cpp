//4F03 PA2
//Michael Dubinets, 1138447
//Mathyvarththan Sivananthan, 1068509

#include <iostream>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

using namespace std;

string alphabet = {"abcdefgh"};
string S;
int stringLength;

void update(char letter, int my_rank) {
   S += letter;
   stringLength++; 
   printf("THREAD %i APPENDED LETTER: %c - SEGMENT S: %s %i \n", my_rank, letter, S.c_str(), stringLength);
}

void constructS() {
   int my_rank = omp_get_thread_num();
   const char letter = alphabet[my_rank];

   srand(time(NULL));
   unsigned int microsleep = rand()%500+100;
   usleep(microsleep);

   #pragma omp critical
   update(letter,my_rank);

}

int main(int argc, char* argv[]) {
   printf("\n---------------PROGRAM START: \n");

   int thread_count = strtol(argv[1], NULL, 10);

   #pragma omp parallel num_threads(thread_count)
   constructS();

   printf("--------String: %s\n", S.c_str());

   return 0;
}