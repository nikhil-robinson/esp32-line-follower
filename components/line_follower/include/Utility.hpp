#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>

template <typename T> T max(T a, T b) { return (a > b) ? a : b; }
template <typename T> T min(T a, T b) { return (a < b) ? a : b; }

template <typename T> T clamp(T Value, T Min, T Max) {
  return (Value < Min) ? Min : (Value > Max) ? Max : Value;
}

template <typename T> T map(T Value, T in_min, T in_max, T out_min, T out_max) {
  return ((Value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

#endif