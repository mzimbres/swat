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

// ROOT

#include "TMath.h"
#include "TVirtualFFT.h"

// SWAT

#include "TWignerd.h"
#include "TSmallWignerd.h"

using namespace TMath;

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Calculate small Wigner-d functions using sine and cosine 
  // transforms(see TVirtualFFT::SineCosine() for details).
  // Uses the transform type 0 and 5 for cosine and sine transforms
  // respectively.
  // See tutorials form details on how to use this class.
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////


//_____________________________________________________________________
TSmallWignerd::TSmallWignerd(Int_t L): 
fSize(2*L), fEvenType(1), fOddType(6)
{
   // Constructor
   // Size of the transform will be 2*L. 

   fEven.reset(TVirtualFFT::SineCosine(1,&fSize,&fEvenType,"M K"));
   fOdd.reset(TVirtualFFT::SineCosine(1,&fSize,&fOddType,"M K"));
}

//_____________________________________________________________________
Double_t* TSmallWignerd::Get(const TWignerd& wig,Int_t m,Int_t n) const
{
   // Will return array with Wigner-d functions. The value l will
   // be taken from wig using TWignerd::GetL(). this value musst be 
   // smaller than L, provided in the constructor.
   

   Int_t l = wig.GetL();

   if (Even(m - n)){

      if ((Abs(n - m) & 3))
         for (Int_t u = 0; u <= l; ++u)
	    fEven->SetPoint(u,-wig.Delta(u,m)*wig.Delta(u,n),0);
      else
	 for (Int_t u = 0; u <= l; ++u)
	    fEven->SetPoint(u,wig.Delta(u,m)*wig.Delta(u,n),0);

      for (Int_t u = l + 1; u < fSize; ++u)
	 fEven->SetPoint(u,0,0);

      fEven->Transform();
      return fEven->GetPointsReal();
   } else {

      if ((Abs(n - m + 1) & 3))
         for (Int_t u = 0; u < l; ++u)
	    fOdd->SetPoint(u,wig.Delta(u + 1,m)*wig.Delta(u + 1,n),0);
      else
	 for (Int_t u = 0; u < l; ++u)
	    fOdd->SetPoint(u,-wig.Delta(u + 1,m)*wig.Delta(u + 1,n),0);

      for (Int_t u = l; u < fSize; ++u)
	 fOdd->SetPoint(u,0,0);

      fOdd->Transform();
      return fOdd->GetPointsReal();
   } 
}


