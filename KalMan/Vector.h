#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <utility>
#include "MISC.h"
#include <fstream>
template <Field T>
class Vector
{
private:
	std::vector<T> x;


public:

   template<std::input_iterator InputIt>
   Vector(InputIt begin, InputIt end)
   {
      x.assign(begin, end);
   }

   Vector(std::vector<T> &&raw_v)
   {
      x = std::move(raw_v);
   }

   Vector(const T *ptr, size_t n)
      : Vector(ptr, ptr + n) {};
   Vector() = default;
   Vector(const Vector &) = default;
   Vector &operator=(const Vector &) = default;
   Vector(Vector &&) noexcept = default;
   Vector &operator=(Vector &&) noexcept = default;
   ~Vector() = default;

   T operator*(const Vector &a) const
   {
      if (x.size() != a.x.size()) {
         throw std::invalid_argument("Vectors has different size");
      }
      T res = T(0);
      for (size_t i = 0; i < x.size(); i++)
      {
         res += x[i] * a.x[i];
      }
      return res;
   }
   Vector operator*(const T &a) const
   {
      std::vector<T> res = x;
      for (size_t i = 0; i < res.size(); i++)
      {
         res[i] *= a;
      }
      return Vector(std::move(res));
   }
   
   Vector operator+(const Vector &a) const
   {
      if (x.size() != a.x.size()) {
         throw std::invalid_argument("Vectors has different size");
      }
      std::vector<T> res(x);
      for (size_t i = 0; i < x.size(); i++)
      {
         res[i] = x[i] + a.x[i];
      }
      return Vector(std::move(res));
   }

   Vector operator-(const Vector &a) const
   {
      if (x.size() != a.x.size()) {
         throw std::invalid_argument("Vectors has different size");
      }
      std::vector<T> res(x);
      for(size_t i = 0; i < x.size(); i++)
      {
         res[i] = x[i] - a.x[i];
      }
      return Vector(std::move(res));
   }

   Vector operator-() const
   {

      std::vector<T> res(x);
      for (size_t i = 0; i < x.size(); i++)
      {
         res[i] = -x[i];
      }
      return Vector(std::move(res));
   }

   void print(std::ostream &stream = std::cout)
   {
      for(size_t i = 0; i < x.size(); i++)
      {
         stream << x[i] << "\t";
      }
      stream << "\n";
   }

   T& operator[](size_t i)
   {
      return x[i];
   }
   T operator[](size_t i) const
   {
      return x[i];
   }

   void expand(size_t n)
   {
      x.resize(x.size() + n);
   }

};

template <Field T>
Vector<T> operator*(const T &a, const Vector<T>& vec)
{
   return vec * a;
}



template <Field T>
Vector<T> random_normal(T loc, T scale, std::size_t size) {
   thread_local std::mt19937_64 gen{ std::random_device{}() };
   std::normal_distribution<T> dist{ loc, scale };

   std::vector<T> result(size);
   std::ranges::generate(result, [&]() { return dist(gen); });

   return Vector<T>(std::move(result));
}
