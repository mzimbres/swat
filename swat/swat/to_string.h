#pragma once 

#include <sstream>
#include <string>

template <typename T>
std::string to_string(T number)
{
  std::ostringstream ss;
  ss << number;
  return ss.str();
}

