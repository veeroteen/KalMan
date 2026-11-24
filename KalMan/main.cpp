#include <iostream>
#include "Matrix.h"
#include "Vector.h"
#include <map>
#include <string>
#include "BasicKalman.h"
#include "Measure.h"
#include <fstream>
#include <functional>
int main()
{
   std::ifstream file("Raw Data.csv");
   std::string trash;
   std::getline(file, trash);
   std::vector<size_t> col = {0, 1,2,3 };
   PreMeasur<double> measure(file, col, ',');
   auto true_fun = [](double t)
      {
         std::vector<double> res = { 0, 0, 0 };
         return Vector<double>(std::move(res));
      };
   BasicCalman calman(&measure, true_fun);
   calman.calc();

   return 0;
}
