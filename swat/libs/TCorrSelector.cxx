#define TCorrSelector_cxx

#include <iostream>
#include <sstream>

#include <TH2.h>
#include <TStyle.h>
#include <TDirectory.h>
#include <TString.h>
#include <THistPainter.h>

#include "TMath.h"
#include "TCanvas.h"
 
#include <swat/swat/TEulerAngle.h>
#include <swat/swat/TCorrSelector.h>

using namespace TMath;
using namespace std;

void TCorrSelector::Begin(TTree * /*tree*/)
{
   J = 8;
   fCount = 0;
   fError = 0.1;

   fEnergy.resize(0);
   fAngle2.resize(0);


   TString filename("corr_graph");
   filename += fCountCalls;
   filename += ".dat";

   corr.open(filename.Data());
   if (!corr.good()){
      cerr << "Failed to open file: " << filename.Data() << endl;
      return;
   }

   corr  << "xcolname ycolname colordata " << fCountCalls << "\n";

   ++fCountCalls;

   out.open("mult_cand.dat",ios_base::app);
   if (!out.good()){
      cerr << "Failed to open file: mult_cand.dat" << endl;
      return;
   }

   out << "%xcolname ycolname colordata " << fCountCalls << "\n";

   TString option = GetOption();
}

void TCorrSelector::SlaveBegin(TTree * /*tree*/)
{

   TString option = GetOption();
}

Bool_t TCorrSelector::Process(Long64_t entry)
{
   // For Herald data in TTree generated with macros/convert_herald.C
   b_six->GetEntry(entry);
   b_seven->GetEntry(entry);
   b_thirtynine->GetEntry(entry);
   Double_t theta = Pi() * (seven + 90.) / 180;
   Double_t phi = Pi() * (six + 180.) / 180;

   if (fAngle->IsInsideRec(theta,phi,fAngSep)) {
       Double_t oneover = 1./thirtynine;
       fEnergy.push_back(oneover); 
       fEnergyError.push_back(thirtynine*fError/Power(thirtynine,2));
       Double_t ang = -fAngSep*180./Pi();
       fAngle2.push_back(ang); 
       fAngle2Error.push_back(1.); 
       ++fCount;
       corr  << oneover  << " " << ang << " " << thirtynine << "\n";

       Double_t x, y;
       THistPainter::ProjectAitoff2xy(-six,seven,x,y);
       out << x << " " << y << " " << thirtynine << "\n";
   }

   return kTRUE;
}

void TCorrSelector::SlaveTerminate()
{

}

void TCorrSelector::Terminate()
{
   corr.close();
   out.close();
   delete fAngle;

   Int_t size = fEnergy.size();

   if (size == 0) {
      //cout << "No points selected." << endl;
      return;
   }

   fGraph = new TGraphErrors(fEnergy.size(),&fEnergy[0],&fAngle2[0],&fEnergyError[0],&fAngle2Error[0]);

   fGraph->GetXaxis()->SetTitle("1/E [EeV^{-1}]");
   fGraph->GetYaxis()->SetTitle("Deflection Angle [degrees]");

   ostringstream oss;
   oss.precision(3);
   oss << fixed << fGraph->GetCorrelationFactor();
   string title = "C = " + oss.str() + ", N = ";
   ostringstream oss1;
   oss1 << fCount;
   title += oss1.str();

   fGraph->SetNameTitle(fGraphName.Data(),title.c_str());

   //fGraph->Fit("pol1");
   gDirectory->Add(fGraph,kTRUE);
   //gDirectory->Add(fMapCut);
}

void TCorrSelector::SetEulerAngles(const TEulerAngle* ang)
{
   fAngle = new TAngDistance(*ang);
}

void TCorrSelector::ShiftAlphaBetaGamma(Double_t a,Double_t b,Double_t c)
{
   fAngle->ShiftAlphaBetaGamma(a,b,c);
}

void TCorrSelector::SetGraphName(const TString& str)
{
   fGraphName = str;
}

void TCorrSelector::SetLengthWidth(Double_t l,Double_t w)
{
   fAngle->SetLength(l);
   fAngle->SetWidth(w);
}
