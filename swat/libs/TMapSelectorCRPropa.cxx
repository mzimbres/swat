#define TMapSelectorCRPropa_cxx

#include <iostream>
#include <string>
#include <cmath>

#include <TMath.h>
#include <TStyle.h>
#include <TDirectory.h>
#include <THistPainter.h>

#include <swat/swat/TMapSelectorCRPropa.h>

using namespace std;
using namespace TMath;

void TMapSelectorCRPropa::Begin(TTree * /*tree*/)
{
   J = 8;
   fCount = 0;

   fMap = new THealpixMap(J);
   fMap->SetNameTitle("hmap","Healpix sky map");

   out.open("skymap.dat");
   if (!out.good()){
      cerr << "Failed to open file datafile.dat" << endl;
      return;
   }

   out << "xcolname ycolname colordata\n";

   TString option = GetOption();

}

void TMapSelectorCRPropa::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();
}

Bool_t TMapSelectorCRPropa::Process(Long64_t entry)
{

   b_Momentum_theta->GetEntry(entry);
   b_Momentum_phi->GetEntry(entry);
   b_Momentum_E_EeV->GetEntry(entry);

   Double_t theta = fmod(static_cast<Double_t>(Momentum_theta) , Pi());
   Double_t phi   = fmod((static_cast<Double_t>(Momentum_phi) + 2*Pi()) , 2*Pi());

   //Double_t theta = static_cast<Double_t>(Momentum_theta);
   //Double_t phi = static_cast<Double_t>(Momentum_phi) + TMath::TwoPi();

   Double_t r = 180./TMath::Pi();
   Double_t six = r*phi - 180.;
   Double_t seven = r*theta - 90.;

   Double_t x, y;
   THistPainter::ProjectAitoff2xy(six,seven,x,y);

   out << x << " " << y << " " << Momentum_E_EeV << "\n";

   fMap->Fill(theta,phi);
   ++fCount;

   return kTRUE;
}

void TMapSelectorCRPropa::SlaveTerminate()
{

}

void TMapSelectorCRPropa::Terminate()
{
   //cout << "Number of events in the map: " << fCount << endl;

   out.close();

   gDirectory->Add(fMap);
}

