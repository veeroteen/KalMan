#pragma once
#include "Vector.h"
#include "MISC.h"
#include <istream>
#include <string>
#include <exception>
template <Field T>
class IMesuarments
{
public:
	virtual ~IMesuarments() = default;
	virtual std::pair<double,Vector<T>> Next() = 0;
   virtual size_t GetAmount() = 0;
};


template<Field T>
class PreMeasur : public IMesuarments<T>
{
private:
	size_t pos = 0;
	std::vector<Vector<T>> measurments;
   std::vector<double> time;

public:
	~PreMeasur() = default;
   std::pair<double, Vector<T>> Next() override
	{
      std::pair<double,Vector<T>> res(time[pos], measurments[pos]);
      pos++;
      return res;
	}
   size_t GetAmount() override
   {
      return measurments.size();
   }


   PreMeasur(std::istream &stream, const std::vector<size_t> &column_num, char separator) {
      std::string buff;
      while (std::getline(stream, buff)) {
         size_t counter = 0;
         auto current_it = buff.begin();
         std::vector<T> result;
         result.reserve(column_num.size());

         for (size_t target_col : column_num) {
            while (counter < target_col) {
               auto sep_it = std::find(current_it, buff.end(), separator);
               if (sep_it == buff.end()) {
                  throw std::runtime_error("unexpected end of string\n");
               }
               counter++;
               current_it = sep_it + 1;
            }


            auto sep_it = std::find(current_it, buff.end(), separator);
            std::string var(current_it, sep_it);
            result.push_back(static_cast<T>(std::stod(var)));

            if (sep_it != buff.end()) {
               current_it = sep_it + 1;
               counter++;
            }
            else if (target_col != column_num.back()) {
               throw std::runtime_error("unexpected end of string\n");
            }
         }
         measurments.emplace_back(result.begin()+1,result.end());
         time.push_back(result[0]);
      }
   }
};