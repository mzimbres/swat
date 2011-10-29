// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C)  2011, Marcelo Zimbres Silva
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

#ifndef SWAT_TStripeAnalysis
#define SWAT_TStripeAnalysis

// ROOT

#include "TSelector.h"

class TString;

// SWAT

class TEulerAngle;

class TStripeAnalysis: public TSelector {
   protected:
   Int_t fCount;
   public:
                 TStripeAnalysis(): fCount(0) {}
   virtual       ~TStripeAnalysis() {}
   virtual void  SetEulerAngles(const TEulerAngle* ang) = 0;
   virtual void  ShiftAlphaBetaGamma(Double_t a,Double_t b,Double_t c) = 0;
   virtual void  SetGraphName(const TString& str) = 0;
   virtual void  SetLengthWidth(Double_t l,Double_t w) = 0;
           Int_t GetCounter() const {return fCount;}
	   void NullCounter() {fCount = 0;}

   ClassDef(TStripeAnalysis,2); // Provide interface for other selector classes.
};

#endif // SWAT_TStripeAnalysis


