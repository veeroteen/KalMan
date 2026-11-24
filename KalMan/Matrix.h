#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <utility>
#include "MISC.h"
#include "Vector.h"
#include <span>
#include <cmath>
template <Field T>
class Matrix {
private:
   std::vector<T> M;
   size_t m=0, n=0;

public:
   template<std::input_iterator InputIt>
   Matrix(InputIt begin, InputIt end, size_t m, size_t n) : m(m), n(n) {
      if (static_cast<size_t>(std::distance(begin, end)) != m * n) {
         throw std::invalid_argument("Input array size is not equal to m*n");
      }
      M.assign(begin, end);
   }

   template<std::input_iterator InputIt>
   Matrix(InputIt begin, InputIt end, size_t n)
      : Matrix(begin, end, n, n) {
   }

   Matrix(std::vector<T> &&raw_m, size_t m, size_t n) : m(m), n(n) {
      if (raw_m.size() != m * n) {
         throw std::invalid_argument("Vector size is not equal to m*n");
      }
      M = std::move(raw_m);
   }
   Matrix(std::vector<T> &&raw_m, size_t n) : m(n), n(n) {
      if (raw_m.size() != n * n) {
         throw std::invalid_argument("Vector size is not equal to n*n");
      }
      M = std::move(raw_m);
   }

   Matrix(const T *ptr, size_t m, size_t n)
      : Matrix(ptr, ptr + (m * n), m, n) {
   }

   Matrix(const T *ptr, size_t n)
      : Matrix(ptr, n, n) {
   }

   Matrix(const Matrix &) = default;
   Matrix &operator=(const Matrix<T> &) = default;
   Matrix(Matrix &&) noexcept = default;
   Matrix &operator=(Matrix<T> &&) noexcept = default;
   ~Matrix() = default;
   Matrix() = default;

   static Matrix<T> GenI(size_t n)
   {
      std::vector<T> res(n * n);
      for (size_t i = 0; i < n; i++)
      {
         res[i + i * n] = 1;
      }
      return Matrix<T>(std::move(res), n);
   }


   Matrix<T> transpose() const {
      std::vector<T> transposed(m * n);
      for (size_t i = 0; i < m; ++i) {
         for (size_t j = 0; j < n; ++j) {
            transposed[i + j * m] = M[j + i * n];
         }
      }
      return Matrix<T>(std::move(transposed), n, m);
   }

   Matrix<T> operator*(const Matrix<T> &B) const {
      if (n != B.m) {
         throw std::invalid_argument("Incompatible matrix dimensions for multiplication");
      }

      std::vector<T> result(m * B.n, T{});

      for (size_t i = 0; i < m; ++i) {
         for (size_t k = 0; k < n; ++k) {
            T a_ik = M[i * n + k];
            for (size_t j = 0; j < B.n; ++j) {
               result[i * B.n + j] += a_ik * B.M[k * B.n + j];
            }
         }
      }
      return Matrix<T>(std::move(result), m, B.n);
   }

   Vector<T> operator*(const Vector<T> &a) const
   {
      std::vector<T> res(m,T(0));
      for (size_t i = 0; i < m; i++)
      {
         for(size_t j = 0; j < n;j++)
         {
            res[i] += (*this)[i][j] * a[j];
         }
      }
      return Vector<T>(std::move(res));
   }

   Matrix<T> operator*(const T &a) const
   {
      std::vector<T> res = M;
      for (size_t i = 0; i < res.size(); i++)
      {
         res[i] *= a;
      }
      return Matrix<T>(std::move(res),m,n);
   }

   Matrix<T> operator+(const Matrix &B) const {
      if (m != B.m || n != B.n) {
         throw std::invalid_argument("Incompatible matrix dimensions");
      }

      std::vector<T> result(m * n, T{});

      for (size_t i = 0; i < m; ++i) {
         for (size_t j = 0; j < n; ++j) {
            result[j + i * n] = M[j + i * n] + B.M[j + i * n];
         }
      }
      return Matrix<T>(std::move(result), m, n);
   }
   Matrix<T> operator-(const Matrix &B) const {
      if (m != B.m || n != B.n) {
         throw std::invalid_argument("Incompatible matrix dimensions");
      }

      std::vector<T> result(m * n, T{});

      for (size_t i = 0; i < m; ++i) {
         for (size_t j = 0; j < n; ++j) {
            result[j + i * n] = M[j + i * n] - B.M[j + i * n];
         }
      }
      return Matrix<T>(std::move(result), m, n);
   }
   void print(std::ostream stream = std::cout)
   {
      for(size_t i = 0; i < m; i++)
      {
         for(size_t j = 0; j < n; j++)
         {
            stream << M[j + i * n] << "\t";
         }
         stream << "\n";
      }
   }

   std::span<T> operator[](size_t row) 
   {
      return { M.data() + row * n, n };
   }
   std::span<const T> operator[](size_t row) const {
      return { M.data() + row * n, n };
   }

   Matrix inverse() const {
      if (m != n) {
         throw std::invalid_argument("Only square matrices can be inverted");
      }
      if (n == 0) {
         return Matrix<T>(std::vector<T>{}, 0);
      }
      std::vector<T> A = M;
      std::vector<T> inv(n * n, T{ 0 });
      for (size_t i = 0; i < n; ++i) {
         inv[i * n + i] = T{ 1 };
      }

      for (size_t i = 0; i < n; ++i) {

         size_t pivot = i;
         if constexpr (std::floating_point<T>) {
            T max_val = std::abs(A[i * n + i]);
            for (size_t r = i + 1; r < n; ++r) {
               T val = std::abs(A[r * n + i]);
               if (val > max_val) {
                  max_val = val;
                  pivot = r;
               }
            }
            if (max_val <= T{ 1e-12 }) {
               throw std::runtime_error("Matrix is singular and cannot be inverted");
            }
         }
         else {
            while (pivot < n && A[pivot * n + i] == T{ 0 }) {
               ++pivot;
            }
            if (pivot == n) {
               throw std::runtime_error("Matrix is singular and cannot be inverted");
            }
         }

         if (pivot != i) {
            for (size_t c = 0; c < n; ++c) {
               std::swap(A[i * n + c], A[pivot * n + c]);
               std::swap(inv[i * n + c], inv[pivot * n + c]);
            }
         }

         T pivot_val = A[i * n + i];
         for (size_t c = 0; c < n; ++c) {
            A[i * n + c] /= pivot_val;
            inv[i * n + c] /= pivot_val;
         }

         for (size_t r = 0; r < n; ++r) {
            if (r != i) {
               T factor = A[r * n + i];
               if (factor != T{ 0 }) {
                  for (size_t c = 0; c < n; ++c) {
                     A[r * n + c] -= factor * A[i * n + c];
                     inv[r * n + c] -= factor * inv[i * n + c];
                  }
               }
            }
         }
      }

      return Matrix<T>(std::move(inv), n);
   }

};

template <Field T>
Matrix<T> operator*(const T &a, const Matrix<T> &matr)
{
   return matr * a;
}