#include <iostream>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include "choose.h"

using namespace std;

char chooseChar(int ranA, int ranC, INPUT inSt) {
   char chosenChar='\0';

   switch (inSt.F) {
      case 0:
      if (counter==0) {
         chosenChar=inSt.c2; occurOfC2++; 
      } else if (counter==1) {
         chosenChar=inSt.c1; occurOfC1++; 
      } else if (counter==2) {
         chosenChar=inSt.c0; occurOfC0++; 
      } else if (occurOfC0 + occurOfC1 < occurOfC2) {
         if (ranC==1) {
            chosenChar=inSt.c0; occurOfC0++;
         } else if (ranC==2) {
            chosenChar=inSt.c1; occurOfC1++; 
         }
      } else if (((occurOfC0 + occurOfC1 >= occurOfC2)&&(inSt.N<4)) || (occurOfC0 + occurOfC1 > occurOfC2)) {
         chosenChar=inSt.c2; occurOfC2++;
      }
      break;
      
      case 1:
      if (counter==0||counter==3||counter==4) {
         chosenChar=inSt.c2; occurOfC2++; 
      } else if (counter==1) {
         chosenChar=inSt.c1; occurOfC1++; 
      } else if (counter==2) {
         chosenChar=inSt.c0; occurOfC0++; 
      } else if (occurOfC0 + 2*occurOfC1 < occurOfC2) {
         if ((counter+2==inSt.L)) {
            chosenChar=inSt.c1; occurOfC1++;}
         else {
            chosenChar=inSt.c0; occurOfC0++;}
      } else if (((occurOfC0 + 2*occurOfC1 >= occurOfC2)&&(inSt.N<4)) ||(occurOfC0 + 2*occurOfC1 > occurOfC2)) {
         chosenChar=inSt.c2; occurOfC2++;
      }
      break;

      case 2:
      if (occurOfC0>0) {
            chosenChar=inSt.c0; occurOfC0--;
      } else if (occurOfC1>0) {
            chosenChar=inSt.c1; occurOfC1--;
      } else if (occurOfC2>0) {
         chosenChar=inSt.c2; occurOfC2--;
      }
      break;
      
      case 3:
      if (counter==0||counter==4) {
            chosenChar=inSt.c2; occurOfC2++;
      } else if (counter==3) {
         chosenChar=inSt.c1; occurOfC1++; 
      } else if (counter==1||counter==2) {
         chosenChar=inSt.c0; occurOfC0++;  
      } else if (occurOfC0 - occurOfC1 < occurOfC2) {
         if (occurOfC0+1 - occurOfC1 < occurOfC2) {
            chosenChar=inSt.c1; occurOfC1++;
         } else if (occurOfC0 - occurOfC1 < occurOfC2+1) {
            chosenChar=inSt.c0; occurOfC0++; }
      } else if (((occurOfC0 - occurOfC1 >= occurOfC2)&&(inSt.N<4)) || (occurOfC0 - occurOfC1 > occurOfC2)) {
         chosenChar=inSt.c2; occurOfC2++;
      }
      break;
   }

   if (chosenChar=='\0') {       
      chosenChar=alphabet[ranA-1];  
   }    

   return chosenChar; 
}