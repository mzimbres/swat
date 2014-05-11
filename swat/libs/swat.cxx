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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <iostream>
#include <memory>
#include <unistd.h>
#include <stdlib.h>

// SWAT

#include <swat/swat/TAlm.h>
#include <swat/swat/TVMap.h>
#include <swat/swat/TAuxFunc.h>

using namespace std;
using namespace TAuxFunc;

void print_usage(const char* progname)
{
   cout << "\n\
   Tests the algorithm performing forward and backward transform.\n\
   Both spherical harmonic transform (if option -N is not provided)\n\
   or spherical wavelet transforms can be performed. I use this program\n\
   to benchmark my code using the time command:\n\
   \n\
   $ time swat -J 8 -N 127\n\
   \n\
   for example. If forward folloed by backward transform do not\n\
   result in the data, with precision 1e-10, program exits with\n\
   EXIT_FAILURE status. For example\n\
   \n\
   $ swat J8 -N 127\n\
   $ echo $?\n\
   0\n\
   $\n\
   \n\
   Usage: " << progname << " [-J j] [-N n]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -J:     Sets band limit of the signal to 2^J, defaults to J = 7.\n\
   -N:     Band limit of wavelet to be used.\n" 
   << endl;
}

int main(int argc,char* argv[])
{
   int J = 7, N = 0;
   char opt;

   while ((opt = getopt(argc,argv,"+hJ:N:")) != -1) {
      switch (opt) {
         case 'J':
	    J = atoi(optarg);
	    break;
         case 'N':
	    if ((N = atoi(optarg)) == 0) {
	       cerr << "N = " << N << ", N must be greaer than 0" << endl;;
	       exit(EXIT_FAILURE);
	    }
	    break;
         default:
	    print_usage(argv[0]);
	    exit(EXIT_SUCCESS);
      }
   }

   auto_ptr<TAlm> alm(TAuxFunc::rand_gaus_alm(J,1,0.3));

   if (N != 0) {
      auto_ptr<TAlm> filtered(alm2wav2alm(*alm.get(),N,0,J));
      if (!compare_alm(*alm.get(),*filtered.get()))
	 exit(EXIT_FAILURE);
   } else {
      TAlm alm2(*alm.get());
      auto_ptr<TVMap> tot(TAuxFunc::SHT(alm2));
      tot->CreateAlm(*alm.get());
      if (!compare_alm(*alm.get(),alm2))
	 exit(EXIT_FAILURE);
   }

   return 0;
}

