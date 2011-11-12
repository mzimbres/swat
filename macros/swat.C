#include <memory>
#include <TRandom.h>
#include <THealpixMap.h>
#include <TMath.h>
#include <TAlm.h>
#include <TWavMap.h>
#include <TEulerAngle.h>
#include <TDirectory.h>
#include <TAuxFunc.h>

TEulerAngle* nonoise();

void swat(int nevents = 1000, int repetition = 100) 
{
   auto_ptr<TEulerAngle> initial(nonoise());
   int equal = 0; 

   for (int j = 0;j < repetition; ++j) {
      TRandom a(j);
      auto_ptr<THealpixMap> hmap((THealpixMap*)gDirectory->Get("hmap"));
      double x,y,z;
      double theta, phi;
      for (int i = 0; i < nevents; ++i) {
	 a.Sphere(x,y,z,1);
	 theta = TMath::ACos(z);
	 phi = TMath::ATan(y/x);
	 hmap->Fill(theta,phi);
      }
      TAlm alm(8);
      hmap->CreateAlm(alm);
      auto_ptr<TWavMap> wav(TAuxFunc::SWAT(alm,2,127));
      wav->FindSources(1,1);
      auto_ptr<TEulerAngle> euler((TEulerAngle*)gDirectory->Get("source0"));
      if (euler->IsEqual(*initial.get()))
         ++equal;
      euler->Show(0);
   }

   cout << "Found to be equal: " << equal << endl;
   cout << "Percentage of unequal: " << (1 - static_cast<double>(equal)/repetition)*100 << "%" << endl;
}

TEulerAngle* nonoise()
{
   auto_ptr<THealpixMap> hmap((THealpixMap*)gDirectory->Get("hmap"));
   TAlm alm(8);
   hmap->CreateAlm(alm);
   auto_ptr<TWavMap> wav(TAuxFunc::SWAT(alm,2,127));
   wav->FindSources(1,1);
   return (TEulerAngle*) gDirectory->Get("source0");
}


