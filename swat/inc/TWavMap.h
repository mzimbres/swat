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

#include <functional>
#include <cmath>

// ROOT

#include "TArrayD.h"

class TF1;
class TH2D;

// SWAT

#include "swat.h"
#include "TCoeffInfo.h"
#include "TDKernel.h"

class TAlm;

class TWavMap: public TCoeffInfo, public TDKernel, public TArrayD {
   private:
   Int_t fOrientation;
   template<typename T>
   struct less_abs: std::binary_function<T,T,bool>{
      bool operator()(T x,T y) const { return std::abs(x) < std::abs(y); }
   };

   public:
             TWavMap(Int_t j,Int_t J,Int_t N = 1): 
	     TCoeffInfo(J,N), TDKernel(j,J), TArrayD(fSizeCoordinate), fOrientation(0)  {}
   Double_t& operator()(Int_t i,Int_t j,Int_t k) {return fArray[Coordinate(i,j,k)];}
   Double_t  operator()(Int_t i,Int_t j,Int_t k) const {return fArray[Coordinate(i,j,k)];}
   void      Filter(Double_t factor);
   void      SetOrientation(Int_t i) {fOrientation = i;}
   void      FindSources(Int_t nsources,Double_t r) const;
   void      CreateAlm(TAlm& alm) const;
   TH2D*     CreateHist(Int_t n) const;
   void      Fill(const TF1& f);
};

#endif // SWAT_TWavMap

