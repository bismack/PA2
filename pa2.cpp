//4F03 PA2
//Michael Dubinets, 1138447
//Mathyvarththan Sivananthan, 1068509

#include <iostream>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>

#ifdef DEBUG
   #define D(x)
#else
   #define D(x) x
#endif

#include "choose.h"
#include "verify.h"
#include "valinput.h"

using namespace std;

string S;
omp_lock_t mutex;
int counter = 0;
int currStringLength = 0;
int totalStringLength;
int numOfSegmentsSatisfied = 0;
bool stringLengthMaxed = false;
int occurOfC0, occurOfC1, occurOfC2;
string alphabet = {"abcdefgh"};

INPUT inSt;

void writeOutputFile() {
   ofstream textFile;
   textFile.open("out.txt");
   textFile << S << endl;
   textFile << numOfSegmentsSatisfied << endl;
   textFile.close();
}

void update(char letter, int thread_id) {
   S += letter;
   currStringLength++;

   if (currStringLength == totalStringLength) {
      stringLengthMaxed = true; 
   }
   
   D(printf("THREAD %i APPENDED LETTER: %c - SEGMENT S: %s %i \n", thread_id, letter, S.c_str(), currStringLength));
}

void constructS() {
   int thread_id = omp_get_thread_num();
   const char letter = alphabet[thread_id];

   while (stringLengthMaxed == false) {
      unsigned int microsleep = rand() % 500 + 100;
      usleep(microsleep);

      if (!stringLengthMaxed) {
         #pragma omp critical
         update(letter,thread_id);
      }
   }
}

void checkSegmentProp() {
   int thread_id = omp_get_thread_num();
   int start = thread_id * inSt.L + counter * inSt.L * inSt.N;
   
   string strSeg = S.substr(start, inSt.L);

   omp_set_lock(&mutex);
   if (verifyF(inSt.F, inSt.c0, inSt.c1, inSt.c2, strSeg) == true) {
      numOfSegmentsSatisfied++;
   }
   omp_unset_lock(&mutex);

   D(printf("-----THREAD: %i Substr: %s   Satisfied: %i\n", thread_id, strSeg.c_str(), numOfSegmentsSatisfied)); 

}

void checkString(int M, int N) {
   while (counter < M/N) {
      #pragma omp parallel num_threads(N)
      checkSegmentProp();
      counter++;
   }
   if ((counter==M/N) && (M%N!=0)) {
      #pragma omp parallel for
      for (int i=0; i < M%N; i++){
         checkSegmentProp();
      }
   }
   writeOutputFile();
}

void constructSE() {
   D(int thread_id = omp_get_thread_num());

   while (counter<inSt.L) {
   unsigned int distrA = (inSt.N<=4) ? 4 : rand()%(inSt.N-3)+4;
   unsigned int distrC = rand()%2+1;

   omp_set_lock(&mutex);
      char letter = chooseChar(distrA, distrC, inSt);
      counter++;
   omp_unset_lock(&mutex);

   unsigned int distrB = rand()%500+100;
   usleep(distrB);

   omp_set_lock(&mutex);
      S += letter;      
      currStringLength++;
      D(printf("THREAD %i APPENDED LETTER: %c - SEGMENT S: %s %i   ---- %i \n", thread_id, letter, S.c_str(), currStringLength, counter));
   omp_unset_lock(&mutex);
   }
}

int main(int argc, char* argv[]) {
   printf("\n---------------PROGRAM START: \n");

   inSt.F = strtol(argv[1], NULL, 10);
   int N = strtol(argv[2], NULL, 10);
   int L = strtol(argv[3], NULL, 10);
   int M = strtol(argv[4], NULL, 10);
   inSt.c0 = argv[5][0];
   inSt.c1 = argv[6][0]; 
   inSt.c2 = argv[7][0];

   totalStringLength = M * L;

   if (!validateInput(inSt.F, N, L, M)) {
   	printf("Exiting program...\n");
   	return 0;
   }

   // generates a seed of pseudo-random numbers
   srand(time(0));
   #pragma omp parallel num_threads(N)
   constructS();
   printf("\n---------------String S CREATED: %s\n", S.c_str());

   inSt.L = L;
   inSt.N = N;

   //check the randomly generated string
   checkString(M, N);

   //ENFORCING S
   printf("\n---------------ENFORCING S: \n");
   S = "";
   currStringLength = 0;
   stringLengthMaxed = false;
   numOfSegmentsSatisfied = 0;
   int total=0,i=0,j=0;

   if (inSt.F==2) {
      while (j<=(inSt.L-2)){
         for (i=0; i<=j;i++) {
            total=i*j;
            // printf("a: %i, b: %i, c: %i /%i\n",j,i,total,inSt.L);
            if (inSt.L==(i+total+j)){ 
               occurOfC0=j; occurOfC1=i; occurOfC2=total;   
               printf("---------->Factors = a: %i, b: %i, c: %i\n",occurOfC0, occurOfC1, occurOfC2);
               goto END;
            }
         }
         j++;
      }
   }
   
   END:
   while (numOfSegmentsSatisfied < M) {
      counter=0;
      occurOfC0=j; occurOfC1=i; occurOfC2=total;
      #pragma omp parallel num_threads(N)
      constructSE();

      numOfSegmentsSatisfied++;
   }
   numOfSegmentsSatisfied = 0;
   counter=0;
   printf("\n---------------String S CREATED: %s\n", S.c_str());

   //check the enforced string
   checkString(M, N);

   return 0;
}
