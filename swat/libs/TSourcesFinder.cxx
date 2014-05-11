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
#include <cstdlib>

// ROOT

#include "TObject.h"
#include "TTree.h"
#include "TEventList.h"
#include "TSelector.h"
#include "TDirectory.h"

#include <swat/swat/THealpixMap.h>
#include <swat/swat/TAlm.h>
#include <swat/swat/TWavMap.h>
#include <swat/swat/TAuxFunc.h>
#include <swat/swat/TSourcesFinder.h>
#include <swat/swat/TMapSelector.h>
#include <swat/swat/TMapSelectorCRPropa.h>

 ///////////////////////////////////////////////////////////////////////
 //                                                    
 // Class which implements the algorithm to find sources in a Sky map 
 // using the wavelet transform.
 //                                                 
 ///////////////////////////////////////////////////////////////////////

//const char* TSourcesFinder::fHeraldCut = "(three < 60.) && (twentythree > 1.) && (twentytwo > 0.5)";
const char* TSourcesFinder::fHeraldCut = "(two > 0) && (three < 60) && (twentytwo > 0.5) && \
(((twentysix > 0.5) && (thirtysix > 0.5)) || (twentythree > 1.1))";

//_________________________________________________________________
TSourcesFinder::TSourcesFinder()
: fNSources(15)
, fN(1)
, fScale(1)
, fSep(3.)
, fMaxEnergy("40.")
, fMinEnergy("15.")
, fMaxTime("1072959036")
, fMinTime("1072959036")
{
   // Will open file and get a pointer to the TTree called events.
   // Very important: This function must be called after setting cuts.

   TObject* obj = gDirectory->Get("events");

   if (!obj) {
      std::cerr << "Events object not in the current directory." << std::endl;
      throw "TSourcesFile: File type not supported.";
   }

   if (!obj->InheritsFrom("TTree")){
      std::cerr << "Object with name events is not a TTree." << std::endl;
      throw "TSourcesFile: No TTree in the directory.";
   }

   fEvents = dynamic_cast<TTree*>(obj);

   std::string title(fEvents->GetTitle());

   if (title == "Data from herald file"){
      fTypeId = Herald;
      //std::cout << "Processing Herald file\n";

   } else if (title == "CRPropa 3D events"){
      fTypeId = CRPropa;
      //std::cout << "Processing CRProa file\n";

   } else {
      std::cerr << "File not supported" << std::endl;
      throw "TSourcesFile: File type not supported.";
   }
}

//__________________________________________________________
void TSourcesFinder::fSetCutHerald(std::string max,std::string min,std::string tmax, std::string tmin)
{
   // Set the string fCuts to new values.

   std::string tcut = "(" + tmin + " < eight && eight < " + tmax + ")";

   fCut = tcut + "&& (" + min + " < thirtynine && thirtynine < " + max + ") && " + fHeraldCut;
}

//__________________________________________________________
void TSourcesFinder::fSetCutCRPropa(std::string max,std::string min)
{
   // Set the string fCuts to new values.

   fCut = min + " < Momentum_E_EeV && Momentum_E_EeV < " + max;
}

//__________________________________________________________________
void TSourcesFinder::GenerateAlm()
{
   // Calculate alm and adds it to the current directory.

   std::auto_ptr<TSelector> fMapSelector;

   switch (fTypeId) {
      case Herald: 
	 fSetCutHerald(fMaxEnergy, fMinEnergy, fMaxTime, fMinTime);
	 fMapSelector.reset(new TMapSelector);
	 break;
      case CRPropa:
	 fSetCutCRPropa(fMaxEnergy,fMinEnergy);
	 fMapSelector.reset(new TMapSelectorCRPropa);
	 break;
   }

   fEvents->Draw(">>list",fCut.c_str());

   TEventList* list = dynamic_cast<TEventList*>(gDirectory->Get("list"));
   fEvents->SetEventList(list);
   fEvents->Process(fMapSelector.get());

   THealpixMap* hmap = dynamic_cast<THealpixMap*>(gDirectory->Get("hmap"));
   TAlm* alm = new TAlm(hmap->GetJ());
   alm->SetName("alm");

   hmap->CreateAlm(*alm);
   gDirectory->Add(alm);
}

//______________________________________________
WavStat TSourcesFinder::FindSources(double wav_threshold)
{
   // Find sources algorithm. 
   // Returns magnitute of biggest wavelet coefficient found.

   GenerateAlm(); // Add alm to the current directory.
   TAlm* alm = dynamic_cast<TAlm*>(gDirectory->Get("alm"));
   if (!alm) {
      std::cerr << "TSourcesFinder::FindSources: Unalbe to retrieve alm." << std::endl;
      std::exit(EXIT_FAILURE);
   }
   std::auto_ptr<TWavMap> wav(TAuxFunc::SWAT(*alm,fScale,fN));

   return wav->FindSources(fNSources, fSep, wav_threshold);
}

