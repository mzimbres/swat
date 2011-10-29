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

// ROOT

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TEventList.h"
#include "TSelector.h"
#include "TDirectory.h"

// SWAT

#include "THealpixMap.h"
#include "TAlm.h"
#include "TVMap.h"
#include "TSourcesFinder.h"
#include "TMapSelector.h"
#include "TMapSelectorCRPropa.h"

ClassImp(TSourcesFinder);


 ///////////////////////////////////////////////////////////////////////
 //                                                    
 // Class which implement the algorithm to find sources in a Sky map 
 // using the wavelet transform.
 //                                                 
 ///////////////////////////////////////////////////////////////////////

const char* TSourcesFinder::fHeraldCut = "(three < 60.) && (twentythree > 1.) && (twentytwo > 0.5)";


//_________________________________________________________________
TSourcesFinder::TSourcesFinder(const char* eventsfile): 
fNSources(15), fN(1), fScale(2), fSep(3.), fMaxEnergy("40."), 
fMinEnergy("15."), fSourcesFile("sources.root")
{
   // Will openfile and get a pointer to the TTree called events.
   // Very important: This function must be called after setting cuts.

   fEventsFile.reset(TFile::Open(eventsfile));
   if (fEventsFile->IsZombie()){
      std::cerr << "File "<< eventsfile <<" not found.";
      throw "TSourcesFile: Events file does not exist.";
   }

   TObject* obj = gDirectory->Get("events");

   if (obj == NULL){
      std::cout << "Events object not in the current directory." << std::endl;
      throw "TSourcesFile: File type not supported.";
   }

   if (!obj->InheritsFrom("TTree")){
      std::cout << "Object with name events is not a TTree." << std::endl;
      throw "TSourcesFile: File type not supported.";
   }

   fEvents = dynamic_cast<TTree*>(obj);

   std::string title(fEvents->GetTitle());

   if (title == "Data from herald file"){
      fTypeId = Herald;
      std::cout << "Processing Herald file\n";

   } else if (title == "CRPropa 3D events"){
      fTypeId = CRPropa;
      std::cout << "Processing CRProa file\n";

   } else {
      std::cerr << "File not supported" << std::endl;
      throw "TSourcesFile: File type not supported.";
   }

}

//__________________________________________________________
void TSourcesFinder::fSetCutHerald(std::string max,std::string min)
{
   // Set the string fCuts to new values.

   fCut = min + " < thirtyeight && thirtyeight < " + max 
          + " && " + fHeraldCut;
}

//__________________________________________________________
void TSourcesFinder::fSetCutCRPropa(std::string max,std::string min)
{
   // Set the string fCuts to new values.

   fCut = min + " < Momentum_E_EeV && Momentum_E_EeV < " + max;
}

//______________________________________________
void TSourcesFinder::FindSources()
{
   // Find sources algorithm. A TTree with name events
   // will be looked for in the current directory
   // and events will be read from it.

   std::auto_ptr<TSelector> fMapSelector;

   switch ( fTypeId ) {
      case Herald: 
	 std::cout << "Setting Cuts ...\n";
	 fSetCutHerald(fMaxEnergy,fMinEnergy);
	 std::cout << "Creating a selector ...\n";
	 fMapSelector.reset(new TMapSelector);
	 break;
      case CRPropa:
	 std::cout << "Setting Cuts ...\n";
	 fSetCutCRPropa(fMaxEnergy,fMinEnergy);
	 std::cout << "Creating a selector ...\n";
	 fMapSelector.reset(new TMapSelectorCRPropa);
	 break;
   }

   std::cout << "Creating an events list ...\n";
   fEvents->Draw(">>list",fCut.c_str());

   TEventList* list = dynamic_cast<TEventList*>(gDirectory->Get("list"));
   fEvents->SetEventList(list);
   std::cout << "Creating the sky map ...\n";
   fEvents->Process(fMapSelector.get());

   THealpixMap* hmap = dynamic_cast<THealpixMap*>(gDirectory->Get("hmap"));
   std::cout << "Transforming to harmonic space ...\n";
   TAlm alm(hmap->GetJ());
   hmap->CreateAlm(alm);
   std::cout << "Transforming to wavelet space ...\n";
   std::auto_ptr<TVMap> wav(alm.SWAT(fScale,fN));

   TFile f(fSourcesFile.c_str(),"recreate");
   std::cout << "Finding sources ...\n";
   wav->FindSources(fNSources,fSep);
   list->Write();
   hmap->Write();

   std::cout << "Saving the sources file ...\n";
   f.Write();
}

