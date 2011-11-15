#include <memory>
#include <TRandom.h>
#include <THealpixMap.h>
#include <TMath.h>
#include <TAlm.h>
#include <TWavMap.h>
#include <TEulerAngle.h>
#include <TDirectory.h>
#include <TAuxFunc.h>

TEulerAngle* nonoise(int,int);

void swat(int nevents = 1000, int repetition = 100,int jj = 2,int N = 1) 
{
   TEulerAngle initial(*nonoise(jj,N));
   initial.Show(0);
   int equal = 0; 

   for (int j = 0;j < repetition; ++j) {
      TRandom a(j);
      auto_ptr<THealpixMap> hmap((THealpixMap*)gDirectory->Get("hmap"));
      double x,y,z;
      double theta, phi;
      for (int i = 0; i < nevents/2; ++i) {
	 a.Sphere(x,y,z,1);
	 theta = TMath::ACos(z);
	 phi = TMath::ATan(y/x) + TMath::Pi()/2;
	 hmap->Fill(theta,phi);
      }
      for (int i = 0; i < nevents/2; ++i) {
	 a.Sphere(x,y,z,1);
	 theta = TMath::ACos(z);
	 phi = TMath::ATan(y/x) - TMath::Pi()/2;
	 hmap->Fill(theta,phi);
      }
      TAlm alm(8);
      hmap->CreateAlm(alm);
      auto_ptr<TWavMap> wav(TAuxFunc::SWAT(alm,jj,N));
      wav->FindSources(1,1);
      TEulerAngle* euler = (TEulerAngle*)gDirectory->Get("source0");
      euler->Show(0);
      bool tt = euler->IsEqual(initial,3,2);
      if (tt)
         ++equal;
      cout << "(" << j << "," << tt << "), "<< flush;
   }
   cout << endl;

   cout << "Found to be equal: " << equal << endl;
   cout << "Percentage of unequal: " << (1. - static_cast<double>(equal)/repetition)*100. << "%" << endl;
}

TEulerAngle* nonoise(int j,int N)
{
   auto_ptr<THealpixMap> hmap((THealpixMap*)gDirectory->Get("hmap"));
   TAlm alm(8);
   hmap->CreateAlm(alm);
   auto_ptr<TWavMap> wav(TAuxFunc::SWAT(alm,j,N));
   wav->FindSources(1,1);
   return (TEulerAngle*) gDirectory->Get("source0");
}


