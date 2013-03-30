#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#include "TSwatB.h"
#include "TAlm.h"
#include "TSkyMap.h"
#include "TSkyMap.h"
#include "TSphHarmF.h"
#include "TCoeffInfo.h"
#include "TMath.h"
#include "to_number.h"

#include <THistPainter.h>

void print_usage(const char* prog)
{
   std::cout << "\n\n\
   Dumps a spherical harmonics in a text file to be ploted\n\
   Usage: " << prog << " [-J scale]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -J:     Maximum scale to perform the analysis.\n\
   -t:     Tolerance to print.\n\
   -l:     l index [0, 2^J)\n\
   -m:     m index 0 <= m <= l\n\
   " << std::endl;
}

int main(int argc, char* argv[])
{
   int J = 5;
   int l = 2 << (J - 1);
   int m = 2 << (J - 1);
   double tol = 0;

   char opt;

   while ((opt = getopt(argc,argv,"+h:J:t:l:m:")) != -1) {
      switch (opt) {
         case 'J':
	    J = to_number<int>(optarg);
	    break;
         case 'l':
	    l = to_number<int>(optarg);
	    break;
         case 'm':
	    m = to_number<int>(optarg);
	    break;
         case 't':
	    tol = to_number<double>(optarg);
	    break;
         default:
	    print_usage(argv[0]);
	    return 1;
      }
   }

   TCoeffInfo info(J, 1);
   int n_theta = info.GetNTheta();
   int n_phi = info.GetNPhi();
   int n_gamma = info.GetNQui();

   TAlm alm(J);
   alm(l,m) = 1;
   TSphHarmF sph(J, "M");
   sph.SetPoints(alm);
   sph.Transform();
   TSkyMap* sky = sph.GetPoints();

   const char* dump_file = "sph_skymap.dat";
   std::ofstream of(dump_file);
   of << "xcol ycol colordata\n";

   const double rad_to_deg = static_cast<double>(180) / TMath::Pi();

   int count = 0;
   for (int i = 0; i < n_phi; ++i) {
     for (int j = 0; j < n_theta / 2; ++j) {
       double phi = 2 * i * TMath::Pi() / n_phi;
       double theta = (2 * j + 1) * TMath::Pi() / n_theta;
       double val = (*sky)(i, j);
       if (std::abs(val) > tol) {
	 double phi_g = rad_to_deg * (phi - TMath::Pi());
	 double theta_g = rad_to_deg * (theta - TMath::Pi() / 2);
	 double x, y;
	 THistPainter::ProjectAitoff2xy(phi_g , theta_g, x, y);
	 of << x << " " << y << " " << val << "\n";
	 ++count;
       } else {
	 std::cout << val << " " << tol << std::endl;
       }
     }
   }
   std::cout << "Number of events in file: " << count << std::endl;

   return 0;
}

