//4F03 PA1
//Michael Dubinets, 1138447
//Mathyvarththan Sivananthan, 1068509

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <time.h>
#include <fstream>
#include <random>

#include "verify.h"
#include "valinput.h"

using namespace std;

//global parameters
pthread_mutex_t mutex;
string alphabet = {"abcdefgh"};
string S;
int numOfSegmentsSatisfied = 0;
int counter = 0;
int stringLength = 0;
int occurOfC0, occurOfC1, occurOfC2;

struct INPUT {
   int F;
   int N;
   int L;
   char c0, c1, c2;
};    
INPUT inSt;

//function for writing output to a .txt file
void writeOutputFile() {
   ofstream textFile;
   textFile.open("out.txt");
   textFile << S << endl;
   textFile << numOfSegmentsSatisfied << endl;
   textFile.close();
}

//function involving threads that cocurrently checks whether or not each substring fits the F criteria
void *checkSegmentProp(void *rank) {
   int tid = (intptr_t)rank;
   int start = tid * inSt.L + counter * inSt.L * inSt.N;
   
   string strSeg = S.substr(start, inSt.L);
   
   if (verifyF(inSt.F, inSt.c0, inSt.c1, inSt.c2, strSeg) == true) {
      pthread_mutex_lock(&mutex);
      numOfSegmentsSatisfied++;
      pthread_mutex_unlock(&mutex);
   }
   printf("-----THREAD: %i Substr: %s   Satisfied: %i\n", tid, strSeg.c_str(), numOfSegmentsSatisfied); 

   return NULL;
}

//function involving threads that concurrently constructs S, with each thread representing a letter according to their rank
void *constructS(void *rank){
   int tid = (intptr_t)rank;
   const char letter = alphabet[tid];

   random_device rd;
   mt19937 eng(rd());
   uniform_int_distribution<> distr(100, 500);
   unsigned int microsleep = distr(eng);

   usleep(microsleep);

   pthread_mutex_lock(&mutex);
   S += letter;
   stringLength++;
   printf("THREAD %i APPENDED LETTER: %c - SEGMENT S: %s %i \n", tid, letter, S.c_str(), stringLength);
   pthread_mutex_unlock(&mutex);

   return NULL;
}

//function for determining which letter is assigned to the threads in creating an enforced string
char choose(int ranA, int ranB, int ranC) {
   char choice;

   switch (inSt.F) {
      case 0:
      if ((S[stringLength]=='\0')&&(stringLength%inSt.L==0)) {
         choice=inSt.c2; occurOfC2++; 
      } else if (occurOfC0 + occurOfC1 < occurOfC2) {
         if (ranC==1) {
            choice=inSt.c0; occurOfC0++;
      } else if (ranC==2) {
            choice=inSt.c1; occurOfC1++; }
      } else if (((occurOfC0 + occurOfC1 >= occurOfC2)&&(inSt.N<4))
      ||(occurOfC0 + occurOfC1 > occurOfC2)) {
         choice=inSt.c2; occurOfC2++;
      }
      break;
      case 1:
      if ((S[stringLength]=='\0')&&(stringLength%inSt.L==0)) {
            choice=inSt.c1; occurOfC1++; 
      } else if (occurOfC0 + 2*occurOfC1 < occurOfC2) {
         if (occurOfC0 + 2*occurOfC1 < occurOfC2+1) {
            choice=inSt.c0; occurOfC0++;}
         else if (occurOfC0 + 2*occurOfC1 < occurOfC2+2){
            choice=inSt.c1; occurOfC1++;}
      } else if (((occurOfC0 + 2*occurOfC1 >= occurOfC2)&&(inSt.N<4))
      ||(occurOfC0 + 2*occurOfC1 > occurOfC2)) {
         choice=inSt.c2; occurOfC2++;
      }
      break;
      case 2:
      if ((S[stringLength]=='\0')&&(stringLength%inSt.L==0)) {
            choice=inSt.c2; occurOfC2++; 
      } else if (occurOfC0 * occurOfC1 < occurOfC2) {
         if ((occurOfC0+1) * occurOfC1 == occurOfC2) {
            choice=inSt.c0; occurOfC0++;
      } else if ((occurOfC0 * (occurOfC1+1) == occurOfC2)
         ||((occurOfC0+1) * (occurOfC1+1) == occurOfC2)) {
            choice=inSt.c1; occurOfC1++; }
      } else if (((occurOfC0 * occurOfC1 >= occurOfC2)&&(inSt.N<4))
      ||(occurOfC0 * occurOfC1 > occurOfC2)) {
         choice=inSt.c2; occurOfC2++;
      }
      break;
      case 3:
      if ((S[stringLength]=='\0')&&(stringLength%inSt.L==0)) {
            choice=inSt.c1; occurOfC1++; 
      } else if (occurOfC0 - occurOfC1 < occurOfC2) {
         if (occurOfC0+1 - occurOfC1 < occurOfC2) {
            choice=inSt.c1; occurOfC1++;
      } else if (occurOfC0 - occurOfC1 < occurOfC2+1) {
            choice=inSt.c0; occurOfC0++; }
      } else if (((occurOfC0 - occurOfC1 >= occurOfC2)&&(inSt.N<4))
      ||(occurOfC0 - occurOfC1 > occurOfC2)) {
         choice=inSt.c2; occurOfC2++;
      }
      break;
   }

   if (choice=='\0') {
      choice=alphabet[ranA-1];
   }
   return choice;
}

//function involving threads that concurrently constructs an string S where each substring matches the F criteria
void *constructSE(void *rank){
   int tid = (intptr_t)rank;

   random_device rd;
   mt19937 eng(rd());
   uniform_int_distribution<> distrA(4, ((inSt.N<4) ? 4 : inSt.N) );
   uniform_int_distribution<> distrB(100, 500);
   uniform_int_distribution<> distrC(1, 2);

   pthread_mutex_lock(&mutex);
   char letter = choose(distrA(eng),distrB(eng),distrC(eng));
   stringLength++;
   pthread_mutex_unlock(&mutex);

   unsigned int microsleep = distrB(eng);
   usleep(microsleep);

   pthread_mutex_lock(&mutex);
   S += letter;
   printf("THREAD %i APPENDED LETTER: %c - SEGMENT S: %s %i \n", tid, letter, S.c_str(), stringLength);
   pthread_mutex_unlock(&mutex);

   return NULL;
}

//main function
int main(int argc, char* argv[]) {
   printf("\n---------------PROGRAM START: \n");

//user inputs
   inSt.F = atoi(argv[1]);
   int N = atoi(argv[2]);
   int L = atoi(argv[3]);
   int M = atoi(argv[4]);
   inSt.c0 = argv[5][0];
   inSt.c1 = argv[6][0]; 
   inSt.c2 = argv[7][0];

   if (!validateInput(inSt.F, N, L, M)){
   	printf("Exiting program...\n");
   	return 0;
   }

   pthread_t threads[N];

//loop that creates the threads which will construct S
   int segment = 1;
   while (segment <= M) {
      for (int i=0; i < N; i++) {
         if (counter < L) {   
            pthread_create(&threads[i], NULL, constructS, (void *)(intptr_t)i);
            counter++;
            if (i == (N-1)) {
               i = 0;
            }
         }
      }
      segment++;
      counter = 0;
	}
   counter = 0;

//joins the threads and blocks further execution until finished
   for (int i=0; i < N; i++) {
      pthread_join(threads[i], NULL);
   }

   printf("\n---------------String S CREATED: %s\n", S.c_str());
   inSt.L = L;
   inSt.N = N;

//loop for checking each substring for whether or not it fits F
   while (counter < M/N) {
      for (int i=0; i < N; i++){
         pthread_create(&threads[i], NULL, checkSegmentProp, (void *)(intptr_t)i);
      }
      for (int i=0; i < N; i++) {
         pthread_join(threads[i], NULL);
      }

      counter++;
      if ((counter==M/N) && (M%N!=0)) {
         for (int i=0; i < M%N; i++){
            pthread_create(&threads[i], NULL, checkSegmentProp, (void *)(intptr_t)i);
         }
         for (int i=0; i < N; i++) {
            pthread_join(threads[i], NULL);
         }
      }
   }
   counter = 0;
   writeOutputFile();

//loop constructing enforced string S
   printf("\n---------------ENFORCING S1: \n");
   S = "";
   segment = 1;
   stringLength = 0;
   while (segment <= M) {
      occurOfC0=0; occurOfC1=0; occurOfC2=0;
      for (int i=0; i < N; i++) {
         if (counter < L) {   
            pthread_create(&threads[i], NULL, constructSE, (void *)(intptr_t)i);
            counter++;
            if (i == (N-1)) {
               i = 0;
            }
         }
      }
      segment++;
      counter = 0;
      for (int i=0; i < N; i++) {
         pthread_join(threads[i], NULL);
      }
   }
   counter = 0;
   printf("\n---------------Enforced string S2 CREATED: %s\n", S.c_str());

//loop for again verifying the string S for F, should have all the substring satisfy F
   numOfSegmentsSatisfied = 0;
   while (counter < M/N) {
      for (int i=0; i < N; i++){
         pthread_create(&threads[i], NULL, checkSegmentProp, (void *)(intptr_t)i);
      }
      for (int i=0; i < N; i++) {
         pthread_join(threads[i], NULL);
      }

      counter++;
      if ((counter==M/N) && (M%N!=0)) {
         for (int i=0; i < M%N; i++){
            pthread_create(&threads[i], NULL, checkSegmentProp, (void *)(intptr_t)i);
         }
         for (int i=0; i < N; i++) {
            pthread_join(threads[i], NULL);
         }
      }
   }
   counter = 0;

   pthread_exit(NULL);
}