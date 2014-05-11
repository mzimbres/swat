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

#ifndef SWAT_THealpixMap
#define SWAT_THealpixMap

// ROOT

#include "TArrayD.h"

class TObject;

// SWAT

#include "swat.h"
#include "TCoeffInfo.h"
#include "TVMap.h"

class TAlm;

class THealpixMap: public TVMap, public TArrayD {
   private:
   Int_t    fJ;
   Int_t    fNSide;
   Long_t   fNPix;
   Int_t    fWeightLength;
   Int_t    fWeightVal;

   public:
         THealpixMap(Int_t J = 6);
         ~THealpixMap() {}
   void  Copy(TObject& rhs) const;
   Int_t GetNPix() const {return fNPix;}
   Int_t GetNSide() const {return fNSide;}
   Int_t GetJ() const {return fJ;}
   void  Filter(Double_t factor);
   void  FindSources(Int_t nsources, Double_t r) const;
   void  CreateAlm(TAlm& harm) const;
   void  Fill(const TF1& f);
   void  Fill(Double_t& theta,Double_t& phi);
   TH2D* CreateHist(Int_t /*n*/) const;
   void  Draw(Option_t* option = "aitoff");

   ClassDef(THealpixMap,1); // A simple interface to Healpix maps.
};

#endif // SWAT_THealpixMap

