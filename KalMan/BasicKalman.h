#pragma once
#include "Matrix.h"
#include "Vector.h"
#include <functional>
#include <vector>
#include <fstream>
#include "Measure.h"

class BasicCalman
{
	Matrix<double> F, B, H, P, Q, R, I;
	Vector<double> x, z;

	double dt, sigma;
	IMesuarments<double> *measurments;
	std::function<Vector<double>(double)> true_fun;
public:
	BasicCalman(IMesuarments<double> *measurments, std::function<Vector<double>(double)> true_fun) : measurments(measurments), true_fun(true_fun)
	{
		dt = 0;
		sigma = 1.5;

		x = Vector<double>(std::vector<double>{ 0, 0, 0 });
		z = Vector<double>(std::vector<double>{ 0, 0, 0 });

		F = Matrix<double>::GenI(3);


		H = Matrix<double>::GenI(3);
		R = Matrix<double>::GenI(3) * pow(sigma, 2);
		Q = Matrix<double>::GenI(3) * 0.01;
	}
	
	void calc()
	{
		std::vector<Vector<double>> results;
		std::vector<Vector<double>> noisyResult;
		std::vector<Vector<double>> rawResult;
		Vector<double> x_est(std::vector<double>{ 0, 0, 0 });
		P = Matrix<double>::GenI(3) * 10.0;


		for (size_t k = 0; k < measurments->GetAmount(); k++)
		{

			Vector<double> X_preq = (k > 0) ? (F * x_est) : x_est;
			Matrix<double> P_preq = (k > 0) ? (F * P * F.transpose() + Q) : P;
			auto meas = measurments->Next();
			z = meas.second + random_normal<double>(0, sigma, 3);
			noisyResult.push_back(z);
			rawResult.push_back(true_fun(meas.first));
			Vector<double> y_res = z - (H * X_preq);
			Matrix<double> S = H * P_preq * H.transpose() + R;
			Matrix<double> K = P_preq * H.transpose() * S.inverse();

			x_est = X_preq + K * y_res;
			P = (Matrix<double>::GenI(3) - K * H) * P_preq;

			results.push_back(x_est);
		}


		std::ofstream fresult("est.csv");
		for (auto &a : results)
		{
			a.print(fresult);
		}
		fresult.close();
		std::ofstream fnresult("meas.csv");
		for (auto &a : noisyResult)
		{
			a.print(fnresult);
		}
		fnresult.close();
		std::ofstream frresult("true.csv");
		for (auto &a : rawResult)
		{
			a.print(frresult);
		}
		frresult.close();
	}
	
};
