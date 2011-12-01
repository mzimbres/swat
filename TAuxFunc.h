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

class TFile;
class TRandom;
class TF1;
class TDirectory;

// SWAT

#include "TAlm.h"

class TWavMap;
class TVMap;

namespace TAuxFunc {
   TAlm*    alm2wav2alm(const TAlm& alm,int N,
			int jmin = 0,
			int jmax = -1);
   void     find_sources(const char* emin = "15.",
                         const char* emax = "40.",
                         int N = 1,
			 int j = 2,
			 int nsources = 15, 
			 double w = 3.,
			 const char* filename = "chain.root");
   void     find_multiplets(double l = 6.,
			    double w = 1.,
			    const char* sourcesfile = "sources.root",
			    const char* filename = "chain.root");
   void     n_vs_gamma(double l = 6.,
		       double w = 1.,
		       const char* sourcesfile = "sources.root",
		       const char* filename = "chain.root");
			    
   bool     compare_alm(const TAlm& alm1,const TAlm& alm2);
   TAlm*    rand_gaus_alm(int J,double mean,double sigma);
   TWavMap* SWAT(const TAlm& alm,int j,int N);
   TVMap*   SHT(const TAlm& alm,bool healpixmap = false);
   void     gensky(int n,TRandom& a,TF1* e);
   void     gensky_from(int n,TRandom& a,TF1* e);
}

#endif

