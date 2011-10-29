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

#ifndef SWAT_TWavMap
#define SWAT_TWavMap

// ROOT

#include "TArrayD.h"

class TF1;
class TObject;

// SWAT

#include "swat.h"
#include "TVMap.h"
#include "TCoeffInfo.h"
#include "TDKernel.h"

class TAlm;

class TWavMap: public TVMap, public TCoeffInfo, public TDKernel, public TArrayD {
   private:

   public:
             TWavMap(Int_t j,Int_t J,Int_t N = 1): 
	     TCoeffInfo(J,N), TDKernel(j,J), TArrayD(fSizeCoordinate) {}
   void      Copy(TObject& rhs) const;
   Double_t& operator()(Int_t i,Int_t j,Int_t k) {return fArray[Coordinate(i,j,k)];}
   Double_t  operator()(Int_t i,Int_t j,Int_t k) const {return fArray[Coordinate(i,j,k)];}
   void      Filter(Double_t factor);
   void      FindSources(Int_t nsources,Double_t r) const;
   void      CreateAlm(TAlm& alm) const;
   TH2D*     CreateHist(Int_t n) const;
   void      Fill(const TF1& f);

   ClassDef(TWavMap,1); // Wavelet coefficients
};

#endif // SWAT_TWavMap

