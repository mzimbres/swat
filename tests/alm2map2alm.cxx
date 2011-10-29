#include <iostream>

// ROOT

#include "TBenchmark.h"
#include "TMath.h"
#include "TRandom.h"
#include "TString.h"

// SWAT

#include "TAlm.h"
#include "TSphHarmF.h"
#include "TSphHarmB.h"
#include "TVMap.h"
#include "TAuxFunc.h"

using namespace std;
using namespace TMath;
using namespace TAuxFunc;

int main(int argv,char* argc[])
{
   // J: Total number of scales.
   // Test the algorithm. A TAlm is filled with 
   // random numbers, then a forward an backward transform 
   // is done ss times on the same data. Int the end values are 
   // compared and the maximum difference is printed on the screen.
   // 
   // Bellow are some values I have obtained:
   //
   // (J,ss) = difference
   // (6,1000) = 2.01e-11

   Int_t J = 7;

   Int_t mean = 0;
   Int_t rms = 1;

   TAlm* alm1 = rand_gaus_alm(J,mean,rms);

   TAlm alm2(*alm1);

   TVMap* tot = alm2.SHT();
   tot->CreateAlm(*alm1);
   delete tot;

   bool equal = compare_alm(*alm1,alm2);
   delete alm1;

   if (equal == false)
      return 1;

   return 0;
} 

