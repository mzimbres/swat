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
#include <vector>

// SWAT

#include "TAlm.h" 

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

   for (size_t i = 0; i < fSize; ++i)
      fAlm[i] += alm.fAlm[i];
}

//________________________________________________________________
void TAlm::Conjugate()
{
   // Conjugates harmonic coefficients

   for (size_t i = 0; i < fSize; ++i)
      fAlm[i] = conj(fAlm[i]);
   
}

//________________________________________________________
void TAlm::ScaleL(const vector<double>& d)
{
   // Dilate spherical harmonics with vector d, that 
   // must have the size equal to l band limit.
   // Multiplies all elements with a given l by d[l]
   // alm(l,m) = d[l]*alm(l,m)

   for (size_t l = 0; l < fL; ++l)
      for (size_t m = 0 ; m <= l; ++m)
         fAlm[fIndex(l,m)] *= d[l];
}

//________________________________________________________
void TAlm::Scale(double factor)
{
   // Multiplies all elements by factor
   // alm(l,m) = factor*alm(l,m)

   for(size_t l = 0; l < fL; ++l)
      for(size_t m = 0; m <= l; ++m)
         fAlm[fIndex(l,m)] *= factor;
}

