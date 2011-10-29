//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 16 14:51:18 2011 by ROOT version 5.27/04
// from TTree events/CRPropa 3D events
// found on file: no_sophia_B1_d5_r002_emin1.root
//////////////////////////////////////////////////////////

#ifndef TCounterCRPropa_h
#define TCounterCRPropa_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "TAngDistance.h"
#include "TStripeAnalysis.h"

class TCounterCRPropa : public TStripeAnalysis {
   public :
   Double_t               fAngSep;
   TAngDistance*          fAngle;
   void SetEulerAngles(const TEulerAngle* ang);
   void ShiftAlphaBetaGamma(Double_t a,Double_t b,Double_t c);
   void SetLengthWidth(Double_t l,Double_t w);
   void SetGraphName(const TString& str) {};

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Float_t         Particle_Type;
   Float_t         Initial_Type;
   Float_t         Initial_Position_X_Mpc;
   Float_t         Initial_Position_Y_Mpc;
   Float_t         Initial_Position_Z_Mpc;
   Float_t         Initial_Momentum_E_EeV;
   Float_t         Initial_Momentum_theta;
   Float_t         Initial_Momentum_phi;
   Float_t         Time_Mpc;
   Float_t         Position_X_Mpc;
   Float_t         Position_Y_Mpc;
   Float_t         Position_Z_Mpc;
   Float_t         Momentum_E_EeV;
   Float_t         Momentum_theta;
   Float_t         Momentum_phi;

   // List of branches
   TBranch        *b_Particle_Type;   //!
   TBranch        *b_Initial_Type;   //!
   TBranch        *b_Initial_Position_X_Mpc;   //!
   TBranch        *b_Initial_Position_Y_Mpc;   //!
   TBranch        *b_Initial_Position_Z_Mpc;   //!
   TBranch        *b_Initial_Momentum_E_EeV;   //!
   TBranch        *b_Initial_Momentum_theta;   //!
   TBranch        *b_Initial_Momentum_phi;   //!
   TBranch        *b_Time_Mpc;   //!
   TBranch        *b_Position_X_Mpc;   //!
   TBranch        *b_Position_Y_Mpc;   //!
   TBranch        *b_Position_Z_Mpc;   //!
   TBranch        *b_Momentum_E_EeV;   //!
   TBranch        *b_Momentum_theta;   //!
   TBranch        *b_Momentum_phi;   //!

   TCounterCRPropa(TTree * /*tree*/ =0) { }
   virtual ~TCounterCRPropa() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(TCounterCRPropa,0); // Selector to count number od events in stripes.
};

#endif

#ifdef TCounterCRPropa_cxx
void TCounterCRPropa::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Particle_Type", &Particle_Type, &b_Particle_Type);
   fChain->SetBranchAddress("Initial_Type", &Initial_Type, &b_Initial_Type);
   fChain->SetBranchAddress("Initial_Position_X_Mpc", &Initial_Position_X_Mpc, &b_Initial_Position_X_Mpc);
   fChain->SetBranchAddress("Initial_Position_Y_Mpc", &Initial_Position_Y_Mpc, &b_Initial_Position_Y_Mpc);
   fChain->SetBranchAddress("Initial_Position_Z_Mpc", &Initial_Position_Z_Mpc, &b_Initial_Position_Z_Mpc);
   fChain->SetBranchAddress("Initial_Momentum_E_EeV", &Initial_Momentum_E_EeV, &b_Initial_Momentum_E_EeV);
   fChain->SetBranchAddress("Initial_Momentum_theta", &Initial_Momentum_theta, &b_Initial_Momentum_theta);
   fChain->SetBranchAddress("Initial_Momentum_phi", &Initial_Momentum_phi, &b_Initial_Momentum_phi);
   fChain->SetBranchAddress("Time_Mpc", &Time_Mpc, &b_Time_Mpc);
   fChain->SetBranchAddress("Position_X_Mpc", &Position_X_Mpc, &b_Position_X_Mpc);
   fChain->SetBranchAddress("Position_Y_Mpc", &Position_Y_Mpc, &b_Position_Y_Mpc);
   fChain->SetBranchAddress("Position_Z_Mpc", &Position_Z_Mpc, &b_Position_Z_Mpc);
   fChain->SetBranchAddress("Momentum_E_EeV", &Momentum_E_EeV, &b_Momentum_E_EeV);
   fChain->SetBranchAddress("Momentum_theta", &Momentum_theta, &b_Momentum_theta);
   fChain->SetBranchAddress("Momentum_phi", &Momentum_phi, &b_Momentum_phi);
}

Bool_t TCounterCRPropa::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef TCounterCRPropa_cxx
