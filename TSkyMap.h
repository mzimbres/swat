// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010 2011 Marcelo Zimbres Silva
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

#ifndef SWAT_TSkyMap
#define SWAT_TSkyMap

// ROOT

#include "TArrayD.h"

// SWAT

#include "swat.h"
#include "TCoeffInfo.h"
#include "TVMap.h"
#include "TAlm.h"

class TAlm;

class TSkyMap: public TVMap, public TCoeffInfo, public TArrayD {
   private:

   public:
   explicit  TSkyMap(Int_t J): TCoeffInfo(J,1), TArrayD(fSizeCoordinate) {}
   void      Copy(TObject& rhs) const;
   Double_t& operator()(Int_t i,Int_t j) {return fArray[Coordinate(i,j,0)];}
   Double_t  operator()(Int_t i,Int_t j) const {return fArray[Coordinate(i,j,0)];}
   void      Filter(Double_t factor);
   void      FindSources(Int_t nsources, Double_t r) const;
   void      CreateAlm(TAlm& alm) const;
   void      Fill(const TF1& f);
   TH2D*     CreateHist(Int_t /*n*/) const;
   void      Draw(Option_t* option = "aitoff");

   ClassDef(TSkyMap,1); // Sky map in the equidistant cylindircal pixelization.
};

#endif // SWAT_TSkyMap

