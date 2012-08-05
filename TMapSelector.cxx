#define TMapSelector_cxx

#include <iostream>
#include <string>


#include "TMapSelector.h"
#include <TH2.h>
#include <TMath.h>
#include <TStyle.h>
#include <TDirectory.h>
#include <THistPainter.h>

using namespace TMath;
using namespace std;

void TMapSelector::Begin(TTree * /*tree*/)
{
   fJ = 8;
   fCount = 0;
   fMap = new THealpixMap(fJ);
   fMap->SetNameTitle("hmap","Healpix sky map");

   out.open("skymap.dat");
   if (!out.good()){
      cerr << "Failed to open file datafile.dat" << endl;
      return;
   }
   out << "xcolname ycolname colordata\n";

   TString option = GetOption();
}

void TMapSelector::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();

}

Bool_t TMapSelector::Process(Long64_t entry)
{
   // Comment and uncomment as needed

   // For Herald data in TTree generated by macros/convert_herald.C
   b_seven->GetEntry(entry);
   b_six->GetEntry(entry);
   b_thirtynine->GetEntry(entry);

   Double_t theta = Pi() * (seven + 90.) / 180;
   Double_t phi = Pi() * (six + 180.) / 180;

   Double_t x, y;
   THistPainter::ProjectAitoff2xy(-six,seven,x,y);
   out << x << " " << y << " " << thirtynine << "\n";

   fMap->Fill(theta,phi);
   ++fCount;

   return kTRUE;
}

void TMapSelector::SlaveTerminate() { }

void TMapSelector::Terminate()
{
   cout << "Number of events in the map: " << fCount << endl;

   gDirectory->Add(fMap);

}

