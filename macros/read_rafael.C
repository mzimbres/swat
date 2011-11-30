#include <iostream>
#include "TSourcesFinder.h"
#include "TAnalysis.h"
#include "TGraph.h"
#include "TDirectory.h"
#include "TEulerAngle.h"
#include "TMath.h"

using namespace std;

void get_crpropa_tree(TTree* t);

void read_rafael(const char* filename, int N = 1, int j = 1)
{
   TFile f(filename,"update");
   if (f.IsZombie())
      return;

   TTree* t = (TTree*)f.Get("data");
   if (t == NULL)
      return;

   // Will put crpropa tree in current directory.
   get_crpropa_tree(t);

   double w = 3;

   TSourcesFinder finder;
   finder.SetMinEnergy("70");
   finder.SetMaxEnergy("100");
   finder.SetN(N);
   finder.SetScale(j);
   finder.SetNSources(1);
   finder.SetSeparation(w);
   finder.FindSources();
   TEulerAngle* euler  = (TEulerAngle*)gDirectory->Get("source0");
   euler->Show(1);

   TAnalysis analysis(gDirectory,gDirectory);
   analysis.SetLength(5);
   analysis.SetWidth(1);
   analysis.GenDeflectionGraphs();
   TGraph* g = (TGraph*)gDirectory->Get("g0");
   if (!g)
      return;
   cout << "Correlation: " << g->GetCorrelationFactor() << endl;
}


void get_crpropa_tree(TTree* t)
{
   // Declaration of leaf types
   Float_t         SrcId;
   Float_t         Energy;
   Float_t         L_src;
   Float_t         B_src;
   Float_t         X_particle;
   Float_t         Y_particle;
   Float_t         Z_particle;
   Float_t         Hit;
   Float_t         L_evt;
   Float_t         B_evt;
   Float_t         Ang;
   Float_t         ImpactAngle;
   Float_t         ImpactParameter;
   Float_t         Mass;
   Float_t         Charge;
   Float_t         EvtId;
   // List of branches
   TBranch        *b_SrcId;   //!
   TBranch        *b_Energy;   //!
   TBranch        *b_L_src;   //!
   TBranch        *b_B_src;   //!
   TBranch        *b_X_particle;   //!
   TBranch        *b_Y_particle;   //!
   TBranch        *b_Z_particle;   //!
   TBranch        *b_Hit;   //!
   TBranch        *b_L_evt;   //!
   TBranch        *b_B_evt;   //!
   TBranch        *b_Ang;   //!
   TBranch        *b_ImpactAngle;   //!
   TBranch        *b_ImpactParameter;   //!
   TBranch        *b_Mass;   //!
   TBranch        *b_Charge;   //!
   TBranch        *b_EvtId;   //!

   t->SetBranchAddress("SrcId", &SrcId, &b_SrcId);
   t->SetBranchAddress("Energy", &Energy, &b_Energy);
   t->SetBranchAddress("L_src", &L_src, &b_L_src);
   t->SetBranchAddress("B_src", &B_src, &b_B_src);
   t->SetBranchAddress("X_particle", &X_particle, &b_X_particle);
   t->SetBranchAddress("Y_particle", &Y_particle, &b_Y_particle);
   t->SetBranchAddress("Z_particle", &Z_particle, &b_Z_particle);
   t->SetBranchAddress("Hit", &Hit, &b_Hit);
   t->SetBranchAddress("L_evt", &L_evt, &b_L_evt);
   t->SetBranchAddress("B_evt", &B_evt, &b_B_evt);
   t->SetBranchAddress("Ang", &Ang, &b_Ang);
   t->SetBranchAddress("ImpactAngle", &ImpactAngle, &b_ImpactAngle);
   t->SetBranchAddress("ImpactParameter", &ImpactParameter, &b_ImpactParameter);
   t->SetBranchAddress("Mass", &Mass, &b_Mass);
   t->SetBranchAddress("Charge", &Charge, &b_Charge);
   t->SetBranchAddress("EvtId", &EvtId, &b_EvtId);

   TTree* newt = new TTree("events","CRPropa 3D events");
   float y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12,y13,y14,y15;
   newt->Branch("Particle_Type"         ,&y1 , "Particle_Type/F"         ); 
   newt->Branch("Initial_Type"          ,&y2 , "Initial_Type/F"          );
   newt->Branch("Initial_Position_X_Mpc",&y3 , "Initial_Position_X_Mpc/F");
   newt->Branch("Initial_Position_Y_Mpc",&y4 , "Initial_Position_Y_Mpc/F");
   newt->Branch("Initial_Position_Z_Mpc",&y5 , "Initial_Position_Z_Mpc/F");
   newt->Branch("Initial_Momentum_E_EeV",&y6 , "Initial_Momentum_E_EeV/F");
   newt->Branch("Initial_Momentum_theta",&y7 , "Initial_Momentum_theta/F");
   newt->Branch("Initial_Momentum_phi"  ,&y8 , "Initial_Momentum_phi/F"  );
   newt->Branch("Time_Mpc"              ,&y9 , "Time_Mpc/F"              );
   newt->Branch("Position_X_Mpc"        ,&y10,"Position_X_Mpc/F"        );
   newt->Branch("Position_Y_Mpc"        ,&y11,"Position_Y_Mpc/F"        );
   newt->Branch("Position_Z_Mpc"        ,&y12,"Position_Z_Mpc/F"        );
   newt->Branch("Momentum_E_EeV"        ,&y13,"Momentum_E_EeV/F"        );
   newt->Branch("Momentum_theta"        ,&y14,"Momentum_theta/F"        );
   newt->Branch("Momentum_phi"          ,&y15,"Momentum_phi/F"          );

   for (int i = 0; i < t->GetEntries(); ++i) {
      t->GetEntry(i,1);
      ////cout << Energy << " " << L_evt << " " << B_evt << "\n";
      y1  =   SrcId;
      y2  =   Energy;
      y3  =   L_src;
      y4  =   B_src;
      y5  =   X_particle;
      y6  =   Y_particle;
      y7  =   Z_particle;
      y8  =   Hit;
      y9  =   L_evt;
      y10 =   B_evt;
      y11 =   Ang;
      y12 =   Mass;
      y13 =   Energy;
      y14 =   (L_evt + 360.)*TMath::Pi()/180;
      y15 =   (B_evt + 180.)*TMath::Pi()/180;
      newt->Fill();
   }

   gDirectory->Add(newt,kTRUE);
}

