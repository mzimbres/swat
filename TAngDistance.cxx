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

// ROOT

#include "TMath.h"

// SWAT

#include "TAngDistance.h" 

using namespace TMath;

   ////////////////////////////////////////////////////////////
   //                                                        // 
   //    
   //   Facilities to calculate if events are inside a 
   //   rectangle around a certain position. The values 
   //   to be set for the length and width of the rectangle 
   //   should be in degrees. Default values are 5. and 1. 
   //   degres for length and width respectively.
   //                                                        // 
   //                                                        // 
   ////////////////////////////////////////////////////////////

//________________________________________________________________
bool TAngDistance::IsInsideRec(Double_t theta,Double_t phi,Double_t& ang) const
{
   // If Events inside rectangle returns true. theta and phi
   // must be in radians. 
   // The variable ang will be set to the angular separation along the axis.

   Double_t dt = theta - fBeta;
   Double_t dp = phi -   fAlpha;

   Double_t a = dt*Sin(-fGamma);
   Double_t b = dp*Sin(fBeta)*Cos(-fGamma);

   Double_t c = dt*Cos(-fGamma);
   Double_t d = dp*Sin(fBeta)*Sin(-fGamma);

   if (Abs(a + b) < fWidth)
      if (Abs(-c + d) < fLength){
//       Double_t tmp = Sqrt(Power(dt,2) + Power(Sin(fInitPos.fBeta)*dp,2));
//	 if (dt > 0)
//	    ang = tmp;
//	 else
//	    ang = -tmp;
	 
	 Double_t ang_t = Abs(dt/Cos(fGamma));
	 Double_t ang_p = Abs(dp*Sin(fBeta)/Sin(fGamma));
	 if (dt > 0)
	    ang = Min(ang_t,ang_p);
	 else
	    ang = -Min(ang_t,ang_p);

         return kTRUE;
      }

   return kFALSE;
}

