#include "TFile.h"
#include "TTree.h"
#include "TObjArray.h"
#include <TRandom.h>
#include <TBranch.h>
#include <TMath.h>

#include <iostream>

void add_background(const char* file, int nevents, double e)
{
   // Add homogeneous sky to a crpropa generated TTree.
   // All events have energy e.

   TFile f(file);
   if (f.IsZombie()) {
      cerr << "TAuxFunc::add_background: Problem reading file" << endl;
   }

   float x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15;
   TTree* t = dynamic_cast<TTree*>(f.Get("events"));
   
   t->SetBranchAddress("Particle_Type",&x1); 
   t->SetBranchAddress("Initial_Type",&x2);
   t->SetBranchAddress("Initial_Position_X_Mpc",&x3);
   t->SetBranchAddress("Initial_Position_Y_Mpc",&x4);
   t->SetBranchAddress("Initial_Position_Z_Mpc",&x5);
   t->SetBranchAddress("Initial_Momentum_E_EeV",&x6);
   t->SetBranchAddress("Initial_Momentum_theta",&x7);
   t->SetBranchAddress("Initial_Momentum_phi",&x8);
   t->SetBranchAddress("Time_Mpc",&x9);
   t->SetBranchAddress("Position_X_Mpc",&x10);
   t->SetBranchAddress("Position_Y_Mpc",&x11);
   t->SetBranchAddress("Position_Z_Mpc",&x12);
   t->SetBranchAddress("Momentum_E_EeV",&x13);
   t->SetBranchAddress("Momentum_theta",&x14);
   t->SetBranchAddress("Momentum_phi",&x15);

   TTree newt("events","CRPropa 3D events");
   float y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12,y13,y14,y15;

   newt.Branch("Particle_Type"         ,&y1 , "Particle_Type/F"         ); 
   newt.Branch("Initial_Type"          ,&y2 , "Initial_Type/F"          );
   newt.Branch("Initial_Position_X_Mpc",&y3 , "Initial_Position_X_Mpc/F");
   newt.Branch("Initial_Position_Y_Mpc",&y4 , "Initial_Position_Y_Mpc/F");
   newt.Branch("Initial_Position_Z_Mpc",&y5 , "Initial_Position_Z_Mpc/F");
   newt.Branch("Initial_Momentum_E_EeV",&y6 , "Initial_Momentum_E_EeV/F");
   newt.Branch("Initial_Momentum_theta",&y7 , "Initial_Momentum_theta/F");
   newt.Branch("Initial_Momentum_phi"  ,&y8 , "Initial_Momentum_phi/F"  );
   newt.Branch("Time_Mpc"              ,&y9 , "Time_Mpc/F"              );
   newt.Branch("Position_X_Mpc"        ,&y10,"Position_X_Mpc/F"        );
   newt.Branch("Position_Y_Mpc"        ,&y11,"Position_Y_Mpc/F"        );
   newt.Branch("Position_Z_Mpc"        ,&y12,"Position_Z_Mpc/F"        );
   newt.Branch("Momentum_E_EeV"        ,&y13,"Momentum_E_EeV/F"        );
   newt.Branch("Momentum_theta"        ,&y14,"Momentum_theta/F"        );
   newt.Branch("Momentum_phi"          ,&y15,"Momentum_phi/F"          );

   for (int i = 0; i < t->GetEntries(); ++i) {
      t->GetEntry(i);
      y1  = x1;   
      y2  = x2; 
      y3  = x3; 
      y4  = x4; 
      y5  = x5; 
      y6  = x6; 
      y7  = x7; 
      y8  = x8; 
      y9  = x9; 
      y10 = x10; 
      y11 = x11; 
      y12 = x12; 
      y13 = x13; 
      y14 = x14; 
      y15 = x15; 
      newt.Fill();
   }

   TRandom a(100);

   double x,y,z;
   for (int i = 0; i < nevents/2; ++i) {
      y1  = 1;   
      y2  = 1; 
      y3  = 1; 
      y4  = 1; 
      y5  = 1; 
      y6  = 1; 
      y7  = 1; 
      y8  = 1; 
      y9  = 1; 
      y10 = 10; 
      y11 = 11; 
      y12 = 12; 
      y13 = e; 
      a.Sphere(x,y,z,1);
      y14 = TMath::ACos(z);
      y15 = TMath::ATan(y/x) + TMath::Pi()/2;
      newt.Fill();
   }
   for (int i = 0; i < nevents/2; ++i) {
      y1  = 1;   
      y2  = 1; 
      y3  = 1; 
      y4  = 1; 
      y5  = 1; 
      y6  = 1; 
      y7  = 1; 
      y8  = 1; 
      y9  = 1; 
      y10 = 10; 
      y11 = 11; 
      y12 = 12; 
      y13 = e; 
      a.Sphere(x,y,z,1);
      y14 = TMath::ACos(z);
      y15 = TMath::ATan(y/x) - TMath::Pi()/2;
      newt.Fill();
   }

   TFile newf("newt.root","recreate");
   newt.Write();
}

