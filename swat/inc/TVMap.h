// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010, 2011, Marcelo Zimbres Silva
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

#ifndef SWAT_TVMap
#define SWAT_TVMap

#include <cmath>
#include <functional>

// ROOT

#include "TObject.h"
#include "TNamed.h"

class TH2D;
class TF1;
class TObject;

// SWAT

class TAlm;


class TVMap: public TNamed {
   protected:
   virtual void      Copy(TObject& rhs) const;
   template <typename T>
   struct less_abs: std::binary_function<T,T,bool>{
      bool operator()(T x,T y) const { return std::abs(x) < std::abs(y); }
   };
   Int_t fOrientation;

   public:
                 TVMap(): TNamed(), fOrientation(0) {}
   virtual       ~TVMap() {}
           void  SetOrientation(Int_t i) {fOrientation = i;}
   virtual void  Filter(Double_t factor) = 0;
   virtual void  FindSources(Int_t nsources = 10,Double_t r = 3.) const = 0;
   virtual void  CreateAlm(TAlm& alm) const = 0;
   virtual TH2D* CreateHist(Int_t n) const = 0;
   virtual void  Fill(const TF1& func) = 0;

   ClassDef(TVMap,1); // Abstract class for all map types.
};

#endif // SWAT_TVMap

