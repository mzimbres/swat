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

// SWAT

#include "TCoeffInfo.h" 

   ////////////////////////////////////////////////////////////
   //                                                        // 
   //    
   //   Provides global indexes and other useful informations.
   // 
   //                                                        // 
   //                                                        // 
   ////////////////////////////////////////////////////////////

//________________________________________________________________
void TCoeffInfo::Copy(TCoeffInfo& rhs) const
{
   // Copy this Alm to new one

      rhs.fJ              = fJ; 
      rhs.fL              = fL; 
      rhs.fN              = fN; 
      rhs.fNPhi           = fNPhi; 
      rhs.fNTheta         = fNTheta; 
      rhs.fNQui           = fNQui; 
      rhs.fSizeCoordinate = fSizeCoordinate;
}


