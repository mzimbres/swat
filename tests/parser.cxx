#include <iostream>

// ROOT

#include <iostream>
#include <string>

// SWAT

#include "TParser.h"

using namespace std;

int main()
{
   // J: Total number of scales.
   // N: Azimuth band limit.
   // Test the algorithm. A forward an backward transform 
   // is done to test the algorithm for a signal with a
   // given band limit.

   int argc = 3;
   const char *argv[] = {"--file=alm.root","--number=3","--exist"};
   //argv[0] = "--file=alm.root";
   //argv[1] = "--number=3";
   //argv[2] = "--exist";

   TParser parser(argc,argv);

   string file("--file");
   string filename; 
   if (parser.get<string>(filename,file))
      if (filename != "alm.root")
         return 1;

   string number("--number");
   int    numberval; 
   if (parser.get<int>(numberval,number))
      if (numberval != 3)
         return 1;

   string exist("--exist");
   if (!parser.exist(exist))
      return 1;

   return 0;
}








