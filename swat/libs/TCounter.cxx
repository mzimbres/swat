#define TCounter_cxx

#include "TMath.h"

#include <swat/swat/TEulerAngle.h>
#include <swat/swat/TCounter.h>

using namespace TMath;

void TCounter::Begin(TTree * /*tree*/)
{
   fCount = 0;

   TString option = GetOption();
}

void TCounter::SlaveBegin(TTree * /*tree*/)
{

   TString option = GetOption();

}

Bool_t TCounter::Process(Long64_t entry)
{
   b_six->GetEntry(entry);
   b_seven->GetEntry(entry);

   Double_t theta = Pi() * (seven + 90.) / 180;
   Double_t phi = Pi() * (six + 180.) / 180;

   if (fAngle->IsInsideRec(theta,phi,fAngSep)) 
       ++fCount;

   return kTRUE;
}

void TCounter::SetEulerAngles(const TEulerAngle* ang)
{
   fAngle = new TAngDistance(*ang);
}

void TCounter::ShiftAlphaBetaGamma(Double_t a,Double_t b,Double_t c)
{
   fAngle->ShiftAlphaBetaGamma(a,b,c);
}

void TCounter::SetLengthWidth(Double_t l,Double_t w)
{
   fAngle->SetLength(l);
   fAngle->SetWidth(w);
}
