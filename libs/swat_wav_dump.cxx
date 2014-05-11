#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#include <THistPainter.h>
#include <TMath.h>

#include <swat/TSwatB.h>
#include <swat/TAlm.h>
#include <swat/TWavMap.h>
#include <swat/TSkyMap.h>
#include <swat/TSphHarmF.h>
#include <swat/TCoeffInfo.h>

void print_usage(const char* prog)
{
   std::cout << "\n\n\
   Dumps a wavelet in a text file to be ploted\n\
   Usage: " << prog << " [-J scale] [-j max-scale] [-N azimuth]\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   -J:     Maximum scale to perform the analysis.\n\
   -j:     Scale used.\n\
   -N:     Wavelet azimuth band limit.\n\
   -p:     Negative delta of phi coordinate to print.\n\
   -i:     Positive delta of phi coordinate to print.\n\
   -t:     Negative delta of theta coordinate to print.\n\
   -e:     Positive delta of theta coordinate to print.\n\
   -a:     Index of alpha coordinate. In the range [0, 2^J)\n\
   -b:     Index of beta coordinate. In the range [0, 2^J)\n\
   -g:     Index of gamma coordinate. In the range [0,N)\n\
   " << std::endl;
}

template <typename T>
T to_number(const char* str)
{
  T tmp;
  std::istringstream ss(str);
  ss >> tmp;
  return tmp;
}

int main(int argc, char* argv[])
{
   int J = 8;
   int j = 2;
   int N = 3;
   int alpha = 2 << (J - 2);
   int beta  = 2 << (J - 2);
   int gamma = 0;
   int p = 0;
   int ii = 0;
   int t = 0;
   int e = 0;

   char opt;

   while ((opt = getopt(argc,argv,"+h:J:j:N:p:i:t:e:a:b:g:")) != -1) {
      switch (opt) {
         case 'J':
	    J = to_number<int>(optarg);
	    break;
         case 'j':
	    j = to_number<int>(optarg);
	    break;
         case 'N':
	    N = to_number<int>(optarg);
	    break;
         case 'a':
	    alpha = to_number<int>(optarg);
	    break;
         case 'b':
	    beta = to_number<int>(optarg);
	    break;
         case 'g':
	    gamma = to_number<int>(optarg);
	    break;
         case 'p':
	    p = to_number<int>(optarg);
	    break;
         case 'i':
	    ii = to_number<int>(optarg);
	    break;
         case 't':
	    t = to_number<int>(optarg);
	    break;
         case 'e':
	    e = to_number<int>(optarg);
	    break;
         default:
	    print_usage(argv[0]);
	    return 1;
      }
   }

   TCoeffInfo info(J, N);
   int n_theta = info.GetNTheta();
   int n_phi = info.GetNPhi();
   int n_gamma = info.GetNQui();

   TWavMap wav(j, J, N);
   wav(alpha, beta, gamma) = 1;
   TSwatB  swat(J, N, "M");
   swat.SetPoints(wav);
   swat.Transform();
   TAlm alm(J);
   swat.GetAlm(alm, j);
   TSphHarmF sph(J, "M");
   sph.SetPoints(alm);
   sph.Transform();
   TSkyMap* sky = sph.GetPoints();

   const char* dump_file = "wav_skymap.dat";
   std::ofstream of(dump_file);
   of << "xcol ycol colordata\n";

   const double rad_to_deg = static_cast<double>(180) / TMath::Pi();

   int count = 0;
   int half = n_theta / 2;
   for (int i = (alpha - p); i < (alpha + ii); ++i) {
     for (int j = (beta - t); j < (beta + e); ++j) {
       double phi = 2 * i * TMath::Pi() / n_phi;
       double theta = (2 * j + 1) * TMath::Pi() / n_theta;
       double val = (*sky)(i, j);
       double phi_g = rad_to_deg * (phi - TMath::Pi());
       double theta_g = rad_to_deg * (theta - TMath::Pi() / 2);
       double x, y;
       THistPainter::ProjectAitoff2xy(phi_g , theta_g, x, y);
       of << x << " " << y << " " << val << "\n";
       ++count;
     }
   }
   std::cout << "Number of events in file: " << count << std::endl;

   return 0;
}

