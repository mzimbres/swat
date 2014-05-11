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

#ifndef SWAT_TAngDistance
#define SWAT_TAngDistance

// SWAT

#include "TEulerAngle.h"
#include "swat.h"

class TAngDistance {
   private:
   Double_t fAlpha;
   Double_t fBeta;
   Double_t fGamma;
   Double_t fLength;
   Double_t fWidth;

   public:
        TAngDistance(const TEulerAngle& pos):
	             fAlpha(pos.Alpha()),
	             fBeta(pos.Beta()),
	             fGamma(pos.Gamma()),
		     fLength(5.*pie/180.),
		     fWidth(1.*pie/180.) {}
   void SetLength(Double_t length) {fLength = length*pie/180.;}
   void SetWidth(Double_t width) {fWidth = width*pie/180.;}
   void ShiftAlphaBetaGamma(Double_t a,Double_t b,Double_t c) {fAlpha += a; fBeta += b; fGamma += c;}
   bool IsInsideRec(Double_t theta,Double_t phi,Double_t& ang) const;
   Double_t GetAlpha() const {return fAlpha;}
   Double_t GetBeta() const {return fBeta;}
   Double_t GetGamma() const {return fGamma;}
};

#endif // SWAT_TAngDistance

