#pragma once
#include "Matrix.h"
#include "Vector.h"
#include <functional>
#include <vector>
#include <fstream>


class BasicCalman
{
	Matrix<double> F, B, H, P, Q, R, I;
	Vector<double> x, z;
	std::function<Vector<double>(double)> true_fun;
	double dt, sigma, t0, t1;
public:
	BasicCalman()
	{
		true_fun = [](double t)
			{
				std::vector<double> res = { 1 * t, 2 * t, 3 * t};
				return Vector<double>(std::move(res));
			};
		sigma = 1.5;
		dt = 1;
		t0 = 0;
		t1 = 10;

		x = Vector<double>(std::vector<double>{ 0, 0, 0, 0, 0, 0 });
		z = Vector<double>(std::vector<double>{ 0, 0, 0 });

		std::vector<double> tmp =
		{
			1,0,0,dt,0,0,
			0,1,0,0,dt,0,
			0,0,1,0,0,dt,
			0,0,0,1,0,0,
			0,0,0,0,1,0,
			0,0,0,0,0,1
		};
		F = Matrix<double>(std::move(tmp), 6);

		tmp =
		{
			1,0,0,0,0,0,
			0,1,0,0,0,0,
			0,0,1,0,0,0
		};
		H = Matrix<double>(std::move(tmp), 3,6);
		R = Matrix<double>::GenI(3) * pow(sigma, 2);
		Q = Matrix<double>::GenI(6) * 0.01;
	}
	
	void calc()
	{
		std::vector<Vector<double>> results;
		std::vector<Vector<double>> noisyResult;
		std::vector<Vector<double>> rawResult;
		Vector<double> x_est(std::vector<double>{ 0, 0, 0, 0, 0, 0 });
		P = Matrix<double>::GenI(6) * 10.0;
		size_t steps = (t1 - t0) / dt;

		for (size_t k = 0; k < steps; k++)
		{
			Vector<double> X_preq = (k > 0) ? (F * x_est) : x_est;
			Matrix<double> P_preq = (k > 0) ? (F * P * F.transpose() + Q) : P;

			z = true_fun(t0 + dt * k) + random_normal<double>(0, sigma, 3);
			noisyResult.push_back(z);
			rawResult.push_back(true_fun(t0 + dt * k));
			Vector<double> y_res = z - (H * X_preq);
			Matrix<double> S = H * P_preq * H.transpose() + R;
			Matrix<double> K = P_preq * H.transpose() * S.inverse();

			x_est = X_preq + K * y_res;
			P = (Matrix<double>::GenI(6) - K * H) * P_preq;

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
