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

// ROOT

#include "TObject.h" 

// SWAT

#include <swat/swat/TVMap.h>

ClassImp(TVMap);

 ///////////////////////////////////////////////////////////////////////
 //                                                    
 //  Stores coefficients of functions defined on SO(3) i.e. functions
 //  of the form f(a,b,c) where 0 <= a <= pi, 0 <= b,c <= 2*pi.
 //
 //  Note: The operator oveload of () follows the convention: 
 //  First index: "i" index of the phi coordinate, range[0,fNPhi)
 //  Second index: "j" index of theta coordinate, range[0,fNTheta)
 //  Third index: "k" index of chi coordinte, range[0,fNQui)
 //                                                 
 ///////////////////////////////////////////////////////////////////////

void TVMap::Copy(TObject& rhs) const
{
   // Copy this to rhs.

   TNamed::Copy(rhs);
}

