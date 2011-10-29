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

#ifndef SWAT_TParser
#define SWAT_TParser

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// SWAT

class TParser {
   private:
   std::vector<std::string>   SetupStrings;
   bool        VERBOSE;
   std::string findname(const std::string &name,bool &found);
   void        pre_parsing();
   public:
               TParser();
               TParser(int argc,const char *argv[]);
   void        init(int argc,const char *argv[]);
   void        set_silentmode(bool v = true);
   bool        exist(const std::string &name);
   template<class T> 
   bool        get(T &var, const std::string &name);
};

// ----------------------- Implementation starts here -----------------------

template<class T>
bool TParser::get(T &var, const std::string &name)
{
  bool found = false;
  std::string str = findname(name,found);
  std::istringstream buffer(str);
  if (found) 
     buffer >> var;
  return found;
}

#endif

