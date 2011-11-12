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

#ifndef SWAT_TEulerAngle
#define SWAT_TEulerAngle

#include <iostream>

// ROOT

#include "Rtypes.h"
#include "TNamed.h"

// SWAT

#include "TCoeffInfo.h" 

class TEulerAngle: public TNamed {
   private:
   TCoeffInfo info; // Useful information.
   Int_t fAlphaInd; // Corresponds to phi in the map.
   Int_t fBetaInd;  // Corresponds to theta in the map.
   Int_t fGammaInd; // Gives orientation on the sphere.
   Double_t fAlpha; // Corresponds to phi in the map.
   Double_t fBeta;  // Corresponds to theta in the map.
   Double_t fGamma; // Gives orientation on the sphere.
   Double_t fValue; // Magnitude of wavelet coefficient.

   public:
   TEulerAngle(Int_t J = 0,Int_t N = 0): info(J,N) {}
   Double_t Alpha() const {return fAlpha;}
   Double_t Beta() const {return fBeta;}
   Double_t Gamma() const {return fGamma;}
   Int_t AlphaInd() const {return fAlphaInd;}
   Int_t BetaInd() const {return fBetaInd;}
   Int_t GammaInd() const {return fGammaInd;}
   Double_t Value() const {return fValue;}
   bool IsEqual(const TEulerAngle& ang) const {
      return ((fAlphaInd == ang.fAlphaInd) && (fBetaInd == ang.fBetaInd) && (fGammaInd == ang.fGammaInd));
   }
   void SetAnglesValue(Int_t alpha,Int_t beta,Int_t gamma,Double_t value);
   void Show(Int_t t = 0) const;
   friend ostream& operator<<(ostream& out, const TEulerAngle& ang);

   ClassDef(TEulerAngle,2); // Stores Euler angles.
};

#endif // SWAT_TEulerAngle

