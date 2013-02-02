#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "TDKernel.h"

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
T to_number(const char* str)
{
  T tmp;
  std::istringstream ss(str);
  ss >> tmp;
  return tmp;
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
     return 1;
   }

   int J = to_number<int>(argv[1]);

   std::string prefix = "kernel_J";
   prefix += argv[1];

   for (int j = 0; j <= J; ++j) {
      std::string file_name = prefix;
      file_name += "j";
      file_name += to_string(j);
      file_name += ".dat";
      TDKernel k(j, J);
      std::vector<double> vec = k.GetKernel();
      std::ofstream fs(file_name.c_str());
      fs << "xcol ycol\n";
      for (int i = 0; i < vec.size(); ++i) {
         fs << i << " " << vec[i] << "\n";
      }
   }

   return 0;
}

