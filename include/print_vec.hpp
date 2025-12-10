#pragma once
#include <iostream>
#include <vector>

inline std::ostream& operator<<(std::ostream& out, const std::vector<double>& v) {
  for (double x : v) {
    out << x << "\n";
  }
  return out;
}