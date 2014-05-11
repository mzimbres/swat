// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2011 Marcelo Zimbres Silva
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <iostream>
#include <memory>
#include <string>
#include <fstream>

// ROOT

#include "TDirectory.h"
#include "TTree.h"
#include "TMath.h"
#include "TEventList.h"
#include "TKey.h"
#include "TAxis.h"
#include "TClass.h"

#include <swat/swat/TEulerAngle.h>
#include <swat/swat/TAnalysis.h>
#include <swat/swat/TCounter.h>
#include <swat/swat/TCorrSelector.h>
#include <swat/swat/TCorrSelectorCRPropa.h>
#include <swat/swat/TCounterCRPropa.h>
#include <swat/swat/TStripeAnalysis.h>

 ///////////////////////////////////////////////////////////////////////
 //                                                    
 // Calculates deflection versus 1/E graphs and number of events 
 // versus stripe orientation graphs for all sources in sources file
 // and adds to the sources file.
 //                                                       
 ///////////////////////////////////////////////////////////////////////

using namespace std;

//_____________________________________________________________________
TAnalysis::TAnalysis(TDirectory* sources,TDirectory* data): 
fLength(6), fWidth(1), fSourcesFile(sources), fDataFile(data)
{
   // This constructor assumes a TTree with name events list
   // is in the file fSourcesFile

   TObject* obj = fDataFile->Get("events");

   if (obj == 0)
      throw "No TTree events in the sources file";

   fDataTree = dynamic_cast<TTree*>(obj);

   obj = fSourcesFile->Get("list");

   if (obj == 0)
      throw "No events list in sources file.";

   TEventList *list = dynamic_cast<TEventList*>(fSourcesFile->Get("list"));
   fDataTree->SetEventList(list);

   std::string title(fDataTree->GetTitle());

   if (title == "Data from herald file"){
      fCorr.reset(new TCorrSelector);
      fCorr1.reset(new TCounter);
      std::cout << "Processing Herald file" << endl;
   } else if (title == "CRPropa 3D events"){
      fCorr.reset(new TCorrSelectorCRPropa);
      fCorr1.reset(new TCounterCRPropa);
      //std::cout << "Processing CRPropa file" << endl;
   } else {
      throw "File type not supported";
   }
}

//_____________________________________________________________________
void TAnalysis::GenDeflectionGraphs()
{
   // Calculates deflection versus 1/E graphs.
   // Loops over objects in Memory and disk.

   Int_t count = 0;

   fSourcesFile->cd();
   TListIter iter(gDirectory->GetListOfKeys());
   while (TKey* key = (TKey*)iter.Next()) {
      TClass tmp1(key->GetClassName());
      if (tmp1.InheritsFrom("TEulerAngle")){
         std::auto_ptr<TEulerAngle> ang(dynamic_cast<TEulerAngle*>(fSourcesFile->Get(key->GetName())));
         fCorr->SetEulerAngles(ang.get());
         fCorr->SetLengthWidth(fLength,fWidth); // Must be here.
         TString gname = "g";
         gname += count;
         fCorr->SetGraphName(gname);
         fDataTree->Process(fCorr.get());
         ++count;
      }
   }

   // Iterates over objects in memory
   TListIter memory(gDirectory->GetList());
   while (TObject* ob = memory.Next()) {
      string obname(ob->ClassName());
      if (obname == "TEulerAngle"){
         std::auto_ptr<TEulerAngle> ang(dynamic_cast<TEulerAngle*>(fSourcesFile->Get(ob->GetName())));
         fCorr->SetEulerAngles(ang.get());
         fCorr->SetLengthWidth(fLength,fWidth); // Must be here.
         TString gname = "g";
         gname += count;
         fCorr->SetGraphName(gname);
         fDataTree->Process(fCorr.get());
         ++count;
      }
   }
}


void TAnalysis::CountEvents()
{
   // Calculates number of events versus orientation graphs.
   // WARNING: Does not loop over objects in memory.

   Double_t conv = 180./TMath::Pi();
   Int_t n = 180;
   Double_t rate = 180./n;
   Int_t count = 0;

   TListIter iter(fSourcesFile->GetListOfKeys());
   while (TKey* key = (TKey*)iter.Next()){
      TClass tmp1(key->GetClassName());
      if (tmp1.InheritsFrom("TEulerAngle")){
         auto_ptr<TEulerAngle> ang(dynamic_cast<TEulerAngle*>(fSourcesFile->Get(key->GetName())));
         ang->Show(1);
         fCorr1->SetEulerAngles(ang.get());
         fCorr1->SetLengthWidth(fLength,fWidth); // Must be here.

         TString gname = "m";
         gname += count;
         TGraph* g = new TGraph(n);
         g->SetNameTitle(gname.Data(),"");
         g->GetXaxis()->SetTitle("Orientation(degrees)");
         g->GetYaxis()->SetTitle("Number of events");
         g->SetLineWidth(3);
         g->SetLineColor(kSpring+3);

         TString name("multiplicity");
         name += count;
         name += ".dat";

         fCorr1->ShiftAlphaBetaGamma(0.,0.,-ang->Gamma());
         Int_t max = 0;
         Int_t min = 1000;

         std::ofstream out(name.Data());
         for (Int_t i = 0; i < n; ++i){
            fCorr1->ShiftAlphaBetaGamma(0.,0.,rate/conv);
            fDataTree->Process(fCorr1.get());
            Int_t c = fCorr1->GetCounter();
            if (c < min) min = c;
            if (c > max) max = c;
            Double_t ang_d = rate*i;
            g->SetPoint(i,ang_d,static_cast<Double_t>(c));
            out << ang_d << " " << c << "\n";
         }
         out.close();

         fSourcesFile->Add(g);
         ++count;
      }
   }
}




