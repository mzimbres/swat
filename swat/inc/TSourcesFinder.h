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

#ifndef SWAT_TSourcesFinder
#define SWAT_TSourcesFinder

#include <string>
#include <memory>

// ROOT

#include "TTree.h"
#include "TFile.h"

// SWAT

enum FileType {Herald,CRPropa};

class TSourcesFinder {
   private:
   Int_t fNSources; // Number of sources to save in the file.
   Int_t fN;        // Precision on gamma angle
   Int_t fScale;    // Scale on which analysis will be performed
   Int_t fSep;      // Sources separation
   std::string fMaxEnergy;  //!
   std::string fMinEnergy;  //!
   std::string fSourcesFile; //! File name
   std::string fCut; //! Cuts
   TTree* fEvents; //!
   std::auto_ptr<TFile> fEventsFile; //!
   void fSetCutHerald(std::string max,std::string min);
   void fSetCutCRPropa(std::string max,std::string min);
   FileType fTypeId; //!
   TSourcesFinder(const TSourcesFinder& obj);
   TSourcesFinder& operator=(const TSourcesFinder& obj);

   public:
                TSourcesFinder(const char* eventsfile);
	        ~TSourcesFinder() { };
   static const char* fHeraldCut;  
   void         SetMaxEnergy(const char* emax) { fMaxEnergy = emax; }
   void         SetMinEnergy(const char* emin) { fMinEnergy = emin; }
   void         SetSeparation(Double_t sep) { fSep = sep; }
   void         SetN(Int_t N) { fN = N; }
   void         SetScale(Int_t j) { fScale = j; }
   void         SetNSources(Int_t n) { fNSources = n; }
   void         FindSources();
   ClassDef(TSourcesFinder,1); // Facade for finding sources.
};

#endif // SWAT_TSourcesFinder

