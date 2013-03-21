#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "TDKernel.h"
#include "to_number.h"

void print_usage(const char* prog)
{
   std::cout << "\n\n\
   Generates pgfplots output to draw the wavelet kernel.\n\
   For a given scale J passed, will generate output for all\n\
   possible values o j in different files.\n\
   Usage: " << prog << " max_scale\n\n\
   Options:\n\n\
   -h:     This menu.\n\
   " << std::endl;
}

template <typename T>
std::string to_string(T number)
{
  std::ostringstream ss;
  ss << number;
  return ss.str();
}

int main(int argc,char* argv[])
{
   if (argc != 2) {
     std::cerr << "Please, provida a maximum scale. 0 < J < 10 (hint)." << std::endl;
     print_usage(argv[0]);
     return 1;
   }

   int J = to_number<int>(argv[1]);

   std::string prefix = "kernel_J";
   prefix += argv[1];
   prefix += ".dat";
   std::ofstream fs(prefix.c_str());

   std::vector<std::vector<double> > vecs;

   for (int j = 0; j <= J; ++j) {
      TDKernel k(j, J);
      vecs.push_back(k.GetKernel());
   }

   for (int i = 0; i < vecs[0].size(); ++i) {
      fs << i << " ";
      for (int j = 0; j <= J; ++j) {
         fs << vecs[j][i] << "  ";
      }
      fs << "\n";
   }

   return 0;
}

