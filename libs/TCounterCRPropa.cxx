#define TCounterCRPropa_cxx

#include <cmath>

#include "TMath.h"

#include <swat/TEulerAngle.h>
#include <swat/TCounterCRPropa.h>

using namespace TMath;

void TCounterCRPropa::Begin(TTree * /*tree*/)
{
   fCount = 0;

   TString option = GetOption();

}

void TCounterCRPropa::SlaveBegin(TTree * /*tree*/)
{

   TString option = GetOption();

}

Bool_t TCounterCRPropa::Process(Long64_t entry)
{
   b_Momentum_theta->GetEntry(entry);
   b_Momentum_phi->GetEntry(entry);
   b_Momentum_E_EeV->GetEntry(entry);

   Double_t theta = fmod(static_cast<Double_t>(Momentum_theta) , Pi());
   Double_t phi   = fmod((static_cast<Double_t>(Momentum_phi) + 2*Pi()) , 2*Pi());

   if (fAngle->IsInsideRec(theta,phi,fAngSep)) 
       ++fCount;


   return kTRUE;
}

void TCounterCRPropa::SlaveTerminate() { }

void TCounterCRPropa::Terminate() { }

void TCounterCRPropa::SetEulerAngles(const TEulerAngle* ang)
{
   fAngle = new TAngDistance(*ang);
}

void TCounterCRPropa::ShiftAlphaBetaGamma(Double_t a,Double_t b,Double_t c)
{
   fAngle->ShiftAlphaBetaGamma(a,b,c);
}

void TCounterCRPropa::SetLengthWidth(Double_t l,Double_t w)
{
   fAngle->SetLength(l);
   fAngle->SetWidth(w);
}

