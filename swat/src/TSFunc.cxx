// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com> 

/* Copyright (C) 2009, 2011 Marcelo Zimbres Silva
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

#include <vector>
#include <iostream>
#include <algorithm>

// ROOT

#include "TMath.h"
#include "TF1.h"

// SWAT

#include "swat.h"
#include "TSFunc.h"

using namespace std;
using namespace TMath;

   /////////////////////////////////////////////////////////////
   //                                                         //
   // Class to build the scale function from continuous       //
   // kernel.                                                 //
   //                                                         //
   /////////////////////////////////////////////////////////////

//___________________________________________________________
TSFunc::TSFunc(Int_t J):fJmax(J),fL(Power(kAlpha,fJmax))
{
   // J: The number of scales that will be used to describe the signal.

   fIntegr = TF1("integrand","exp(-2/(1-((2*([0]*[2]*x-[1])/(([0]-1)*[1]))-1)**2))/x");
   fIntegr.SetParameter(0,kAlpha);
   fIntegr.SetParameter(1,fL);
}

//___________________________________________________________
vector<Double_t> TSFunc::GetSquaredAtScale(Int_t s)
{
   // Returns a vector that contains the points 
   // of the squared scale function.
   //
   // s: Is the scale chosen, must be in the range
   // 0 <= s <= jmax

   if (s < 0 || s > fJmax)
      throw "TGetSquaredAtScale::Scale must be in the range j <= J";

   vector<Double_t> Phi2;

   fIntegr.SetParameter(2,Power(kAlpha,s));

   Double_t a = Power(kAlpha,fJmax-s-1);
   Double_t b = Power(kAlpha,fJmax-s);

   Double_t C_Phi = fIntegr.Integral(a,b); 

   Phi2.reserve(fL);

   for(Int_t i = 0; i < fL; ++i){
      if (i <= a){ 
         Phi2.push_back(1);
      } else if (i >= b){ 
          Phi2.push_back(0); 
      } else { 
	 Double_t c = fIntegr.Integral(i,b)/C_Phi; 
	 Phi2.push_back(c);
      }
   }
   return Phi2;
}

//___________________________________________________________
vector<Double_t> TSFunc::GetAtScale(Int_t s){

   // Returns a vector that contains the points 
   // of the scale function.
   //
   // s: Is the scale chosen, must be in the range
   // 0 <= s <= jmax

   vector<Double_t> Phi2;

   if(s<0 || s>fJmax){
      Error("GetAtScale","Scale should be in the range[%d,%d]",0,fJmax);
      return Phi2;
   }

   fIntegr.SetParameter(2,Power(kAlpha,s));

   Double_t a = Power(kAlpha,fJmax-s-1);
   Double_t b = Power(kAlpha,fJmax-s);

   Double_t C_Phi = fIntegr.Integral(a,b); 

   Phi2.reserve(fL);

   for (Int_t i = 0;i < fL;++i){
      if (i <= a) { 
         Phi2.push_back(1);
      } else if (i >= b){
	 Phi2.push_back(0);
      } else { 
	 Double_t c = fIntegr.Integral(i,b)/C_Phi; 
	 Phi2.push_back(c);
      }
   }

   transform(Phi2.begin(),Phi2.end(),Phi2.begin(),Sqrt);
   return Phi2;
}

