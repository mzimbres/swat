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

#include <iostream>

// ROOT

#include "TMath.h"

// SWAT

#include <swat/swat/swat.h>
#include <swat/swat/TEulerAngle.h>

ClassImp(TEulerAngle);

using namespace std;
using namespace TMath;

   ////////////////////////////////////////////////////////////
   //                                                        // 
   //    
   // Stores Euler angles and information about pixelization.
   //   
   //                                                        // 
   //                                                        // 
   ////////////////////////////////////////////////////////////

//________________________________________________________________
void TEulerAngle::SetAnglesValue(Int_t alpha,Int_t beta,Int_t gamma,Double_t value)
{
   // Set angles.

   fAlphaInd = alpha;
   fBetaInd  = beta;
   fGammaInd = gamma;
   fValue    = value;

   fAlpha = 2*alpha*Pi()/info.GetNPhi();
   fBeta  = 2*beta*Pi()/info.GetNTheta();
   fGamma = 2*gamma*Pi()/info.GetNQui();
}


//________________________________________________________________
void TEulerAngle::Show(Int_t t) const
{
   // Print values on the screen. 
   // If equal:
   // 1: Print idexes.
   // 2: Coordinates that follow the same used in the canvas.
   // 3: Data Challenge coordinates.

   Double_t c = 180./TMath::Pi();

   switch (t) {
      
      case 0:
         cout << "wav(" << fAlphaInd << "," << fBetaInd << "," 
	      << fGammaInd << ") = " << fValue << "\n";
	 break;

      case 1:
	 cout << "wav(" << (fAlpha*c - 180.) << "," 
	      << (fBeta*c - 90.) << "," << (fGamma*c)  << ") = "
	      << fValue << "\n";
	 break;
         
      case 2:
	 cout << "wav(" << (fAlpha*c - 0.) << "," 
	      << (fBeta*c - 90.) << "," << (fGamma*c)  << ") = "
	      << fValue << "\n";
	 break;
   }
}

ostream& operator<<(ostream& out, const TEulerAngle& ang)
{
   Double_t c = 180./TMath::Pi();

   out.setf(std::ios::fixed, std::ios::floatfield);
   out.precision(3);   

   out << "wav(" << (ang.fAlpha*c - 180.) << "," << (ang.fBeta*c - 90.) 
        << "," << (ang.fGamma*c)  << ") = " << ang.fValue << "\n";

   return out;
}


