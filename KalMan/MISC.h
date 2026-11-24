//MISC.h
#pragma once
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

template <typename T>
concept Field = requires(T a, T b, std::istream & is, std::ostream & os) {
   { a + b } -> std::same_as<T>;
   { a - b } -> std::same_as<T>;
   { a * b } -> std::same_as<T>;
   { a / b } -> std::same_as<T>;

   { a == b } -> std::convertible_to<bool>;
   { a != b } -> std::convertible_to<bool>;
   { a > b }-> std::convertible_to<bool>;
   { a < b }-> std::convertible_to<bool>;
   { a = b } -> std::same_as<T &>;
   { std::abs(a) };
   { os << a } -> std::same_as<std::ostream &>;
   { is >> a } -> std::same_as<std::istream &>;
};


template<Field T>
T pow(T a, unsigned b)
{
   T res = (b == 0 ? 1 : a);

   for (unsigned i = 1; i < b; i++)
   {
      res *= a;
   }
   return res;
}

