#pragma once
#include "Matrix.h"
#include "Vector.h"
#include <functional>
#include <vector>
#include <fstream>

class AKF
{
	Vector<double> x, z;
	Matrix<double> A,H,P,Q,R;
	double a = -0.8448;
	double dt, t0, t1;
	double b = 0.98; // forgetting coeff
	double alpha = 0.01;
	double Xi = 6.63;
	double c = 1.2;

	AKF(double t0, double t1): t0(t0),t1(t1)
	{
		dt = 1.0 / 200.0;
		x = Vector<double>(std::move(std::vector<double>{ 0,0,0 }));
		z = x;
		A = a * Matrix<double>::GenI(3);
		H = Matrix<double>::GenI(3);
		P = Matrix<double>::GenI(3);
		Q = 1e-5 * Matrix<double>::GenI(3);
		R = 1e-4 * Matrix<double>::GenI(3);
	
	}

	void calc()
	{
		Vector<double> XPreq;
		Vector<double> e;
		Matrix<double> PPreq;
		Matrix<double> S, K;

		size_t steps = (t1 - t0) / dt;
		for(size_t i = 0; i < steps; i++)
		{
			//prediction
			XPreq = A * x;
			PPreq = A * P * A.transpose() + Q;

			//correction
			S = H * PPreq * H.transpose() + R;
			K = PPreq * H.transpose() * S.inverse();
			//e = z - H * XPreq; z need to be evaluated
			x = XPreq + K * e;
			P = (Matrix<double>::GenI(3) - K * H) * PPreq;

			//adaptation
			double dk = (1.0 - b) / (1.0 - pow(b, i + 1));
			Matrix<double> Qsemi = K * e * e * K.transpose();
			Q = (1 - dk) * Q + dk * Qsemi;
			Vector<double> n = 
		}
	
	}


};