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

#ifndef SWAT_TAnalysis
#define SWAT_TAnalysis

#include <memory>

// ROOT

#include "TFile.h"
#include "TTree.h"

// SWAT

#include "TStripeAnalysis.h"

// SWAT
class TAnalysis {
   private:
   Double_t fLength;
   Double_t fWidth;
   TFile    fSourcesFile;
   TFile    fDataFile;
   std::auto_ptr<TTree>   fDataTree;
   std::auto_ptr<TStripeAnalysis> fCorr;  // For deflection calculation.
   std::auto_ptr<TStripeAnalysis> fCorr1; // For couting in stripes.

   public:
   TAnalysis(const char* sources, const char* data);
   void SetLength(Double_t l) {fLength = l;}
   void SetWidth(Double_t w) {fWidth = w;}
   void GenDeflectionGraphs();
   void CountEvents();
};


#endif // SWAT_TAnalysis

