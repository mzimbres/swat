// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010, 2011 Marcelo Zimbres Silva
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

#ifndef SWAT_TAuxFunc
#define SWAT_TAuxFunc

// ROOT

#include "TFile.h"
#include "TWavMap.h"
#include "TArrayD.h"

// SWAT

#include "TAlm.h"
class TVMap;

namespace TAuxFunc {
   TAlm*    alm2wav2alm(const TAlm& alm,Int_t N,
			Int_t jmin = 0,
			Int_t jmax = -1);
   void     find_sources(const char* emin = "15.",
                         const char* emax = "40.",
                         Int_t N = 1,
			 Int_t j = 2,
			 Int_t nsources = 15, 
			 Double_t w = 3.,
			 const char* filename = "chain.root");
   void     find_multiplets(Double_t l = 6.,
			    Double_t w = 1.,
			    const char* sourcesfile = "sources.root",
			    const char* filename = "chain.root");
   void     n_vs_gamma(Double_t l = 6.,
		       Double_t w = 1.,
		       const char* sourcesfile = "sources.root",
		       const char* filename = "chain.root");
			    
   TObject* retrieve(TFile* f,const char* classname);
   bool     valid_file(const TFile& file,const char* classname);
   bool     compare_alm(const TAlm& alm1,const TAlm& alm2);
   TAlm*    rand_gaus_alm(Int_t J,Double_t mean,Double_t sigma);
   TWavMap* SWAT(const TAlm& alm,int j,int N);
}

#endif

