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


#include <sstream>
#include <string>
#include <vector>

// SWAT

#include "TParser.h"

using namespace std;

TParser::TParser()
{
  VERBOSE = true;
}

TParser::TParser(int argc,const char *argv[])
{
  VERBOSE = true;
  init(argc, argv);
}

void TParser::pre_parsing(void)
{
   int i, j, n;
   int size = SetupStrings.size();
   string Line, NewLine;
   std::vector<std::string> TempSetupStrings;

   for (i = 0; i < size; ++i) {
      NewLine = "";
      Line = SetupStrings.at(i);
      n = Line.length();
      j = 0; //counter in Line
      while (j < n) {
        switch (Line[j]) {
        case '\n':
          j++;
          break;
        case ' ':
          j++;
          break;
        case '\t':
          j++;
          break;
        case '%':
          j = n;
          break;
        default:
          NewLine += Line[j];
          j++;
          break;
        }
      }
      SetupStrings.at(i) = NewLine;
  }
}

void TParser::init(int argc,const char *argv[])
{
  SetupStrings.resize(argc);

  for (int i = 0; i < argc; i++) {
    SetupStrings.at(i) = argv[i];
  }
  pre_parsing();
}

void TParser::set_silentmode(bool v)
{
  VERBOSE = !v;
}

bool TParser::exist(const std::string &name)
{
  bool found = false; 
  std::string temp = findname(name,found);
  return found;
}

string TParser::findname(const string &name,bool &found)
{
   string Name = "";
   string Out  = "";
   string Line = "";

   unsigned int i = 0;
   while (i++ < SetupStrings.size()) {
      Line = SetupStrings.at(i-1);
      if (Line.find_first_of("=") != string::npos){
         Name = Line.substr(0, Line.find_first_of("="));
         if (Name == name) {
            found = true;
            //cout << "aaaa " << found << endl;
            string::size_type idx = Line.find_first_of("=") + 1;
            if (idx <= Line.size())
               return Line.substr(idx);
         }
      }

      if (Line.find(name) != string::npos){
	 //cout << "bbbb " << found << endl;
	 found = true;
      }
   }
   return Out;
}

