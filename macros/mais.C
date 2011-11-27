#include <iostream>
#include "TRandom.h"
#include "TF1.h"
#include "TAuxFunc.h"
#include "TSourcesFinder.h"
#include "TAnalysis.h"
#include "TGraph.h"
#include "TDirectory.h"
#include "TList.h"
#include "TKey.h"

using namespace std;

void mais()
{

   //TFile aa("stuff.root","recreate");
   int N = 1, j = 1;
   double w = 3;
   TRandom a(1);
   TF1 f("f","1/pow(x,3)",20,40);

   TAuxFunc::gensky(1000,a,&f);

   TSourcesFinder finder;
   finder.SetMinEnergy("20");
   finder.SetMaxEnergy("40");
   finder.SetN(N);
   finder.SetScale(j);
   finder.SetNSources(1);
   finder.SetSeparation(w);
   finder.FindSources();

   TAnalysis analysis(gDirectory,gDirectory);
   analysis.SetLength(5);
   analysis.SetWidth(1);
   analysis.GenDeflectionGraphs();
   TGraph* g = (TGraph*)gDirectory->Get("g0");
   if (!g)
      return;
   cout << "Correlation: " << g->GetCorrelationFactor() << endl;
}

