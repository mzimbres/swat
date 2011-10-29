// Author: Marcelo Zimbres Silva 2009

/* Copyright (C) 2009, 2010, 2011 Marcelo Zimbres Silva
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
#include <exception>
#include <vector>

// ROOT

#include "TVirtualFFT.h"

// SWAT

#include "TAlm.h" 
#include "TSkyMap.h" 
#include "THealpixMap.h" 
#include "TSphHarmF.h" 

// Healpix

#include "Healpix_Map.h"
#include "arr.h"
#include "alm_map_tools.h"
#include "Alm.h"
#include "pointing.h"
#include "xcomplex.h"
#include "alm_healpix_tools.h"

using namespace std;

 /////////////////////////////////////////////////////////////////////
 //                                                           
 //                                                           
 //  Stores spherical harmonic coefficients(SHC) calculated from real data.
 //  Which means they will have the property a^{*}_{lm} = (-1)^{m}a_{l(-m)}.
 //  The main point in writing a class to store spherical harmonic
 //  coefficients from scratch, instead of using Healpix Alm<T> for example,
 //  is to generate dictionaries easily (I did not try to generated
 //  dictionaries for Healpix classes) and to inherit from TObject to be able
 //  to save the objects to ROOT files.
 //                                                           
 /////////////////////////////////////////////////////////////

//________________________________________________________________________
void TAlm::Add(const TAlm& alm)
{
   // Add alm to this

   for (int i = 0; i < fSize; ++i)
      fAlm[i] += alm.fAlm[i];
}

//________________________________________________________________
void TAlm::Conjugate()
{
   // Conjugates harmonic coefficients

   for (int i = 0; i < fSize; ++i)
      fAlm[i] = conj(fAlm[i]);
   
}

//________________________________________________________
void TAlm::ScaleL(const vector<double>& d)
{
   // Dilate spherical harmonics with vector d, that 
   // must have the size equal to l band limit.
   // Multiplies all elements with a given l by d[l]
   // alm(l,m) = d[l]*alm(l,m)

   for (size_t l=0;l < fL;++l)
      for (size_t m = 0;m <= l; ++m)
         fAlm[fIndex(l,m)] *= d[l];
}

//________________________________________________________
void TAlm::Scale(double factor)
{
   // Multiplies all elements by factor
   // alm(l,m) = factor*alm(l,m)

   for(size_t l = 0; l < fL; ++l)
      for(size_t m = 0; m <= l;++m)
         fAlm[fIndex(l,m)] *= factor;
}

//________________________________________________________
TVMap* TAlm::SHT(bool healpixmap) const
{
   // Performs a spherical harmonic transform. If healpixmap is true
   // create a THealpixMap, otherwise a TSkyMap.

   if (healpixmap) {

      Alm<xcomplex<double> > alm1(fL-1,fL-1);

      xcomplex<double> c;
      for (size_t l = 0; l < fL; ++l){
	 for (size_t m = 0; m <= l; ++m){
	    c.real() = real(fAlm[fIndex(l,m)]);
	    c.imag() = imag(fAlm[fIndex(l,m)]);
	    alm1(l,m) = c;
	 }
      }

      Healpix_Map<double> map(fL,RING,SET_NSIDE);
      alm2map<double>(alm1,map);

      THealpixMap* mapa = new THealpixMap(fJmax);
      mapa->Set(mapa->GetNPix(),&map[0]);

      return mapa;
   } else {
      TSphHarmF forward(fJmax);
      forward.SetPoints(*this);
      forward.Transform();
      return forward.GetPoints();
   }

}

