// Author: Marcelo Zimbres Silva <mailto:mzimbres@gmail.com>

/* Copyright (C) 2010 Marcelo Zimbres Silva
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
#include <complex>
#include <vector>

// SWAT

#include <swat/TWavelet.h>
#include <swat/TAlm.h>
#include <swat/TDKernel.h>
#include <swat/TSlm.h>

using namespace std;

//___________________________________________________________________
void TWavelet::fill_wav(TAlm& alm,int j,int N)
{
    //  Fills alm with wavelet SH defined in:
    //
    //  [1] Exact reconstruction with directional wavelets on the 
    //  sphere, Y. Wiaux, J. D. McEwen, P. Vandergheynst and O. Blanc 

   if (j > alm.GetJmax())
      cout << "fill_wav: j greater than J" << endl;

   TSlm Direc(N);
   TDKernel ker(j,alm.GetJmax());
   
   vector<double> vec = ker.GetKernel();

   for(int l = ker.Begin(); l < ker.End(); ++l){
      int lim = (l < N) ? l: (N - 1);
      for (int m = 0; m <= lim; ++m){
         alm(l,m) = vec[l]*Direc(l,m);
      }
   }
}


