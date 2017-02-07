//4F03 PA2
//Michael Dubinets, 1138447
//Mathyvarththan Sivananthan, 1068509

#include <iostream>
#include <stdio.h>
#include <omp.h>

using namespace std;

void Hello() {
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();
   
   printf("Hello thread: %d of %d\n", my_rank, thread_count);

}

int main(int argc, char* argv[]) {
   printf("\n---------------PROGRAM START: \n");

   int thread_count = strtol(argv[1], NULL, 10);

   #pragma omp parallel
   Hello();
}