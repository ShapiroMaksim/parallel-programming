#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <string>
#include <omp.h>

using namespace std;


double integral(const double LeftX, const double RightX, const double LeftY,
	const double RightY, const double StepX, const double StepY)
{
	//   x*y*sin(x*y)

	double sum = 0.0;
	double midval;

	for (double i = LeftX; i < RightX; i += StepX)
	{
		for (double j = LeftY; j < RightY; j += StepY)
		{

			midval = ((i * j * sin(i * j)) +
				((i + StepX) * j * sin((i + StepX) * j)) +
				(i * (j + StepY) * sin(i * (j + StepY))) +
				((i + StepX) * (j + StepY) * sin((i + StepX) * (j + StepY))))
				/ 4.0; 
			sum += midval * StepX * StepY;

		}
	}
	return sum;

}

int main(int argc, char* argv[])
{
	double LeftX, RightX, LeftY, RightY, StepX, StepY;
	double start, end, omp_start, omp_end;
	double res, omp_res = 0;
	if (argc < 8) {
		return -1;
	}
	LeftX = stof(argv[1]);
	RightX = stof(argv[2]);
	LeftY = stof(argv[3]);
	RightY = stof(argv[4]);
	StepX = stof(argv[5]);
	StepY = stof(argv[6]);
	omp_set_num_threads(stoi(argv[7]));
	int thread_num = stoi(argv[7]);
	start = omp_get_wtime();
	res = integral(LeftX, RightX, LeftY, RightY, StepX, StepY);
	end = omp_get_wtime();
	cout << "linear_res: " << res << endl;
	cout << "linear_time: " << end - start << endl;
	omp_start = omp_get_wtime();


	double* X;
	X = new double[2 * thread_num];
	double tmp = (RightX - LeftX) / (double)thread_num;

	for (int i = 0; i < thread_num * 2; i += 2) {
		X[i] = LeftX + i * tmp / 2;
		X[i + 1] = X[i] + tmp;
	}
#pragma omp parallel for reduction(+:omp_res)
	for (int i = 0; i < thread_num * 2; i += 2) {

		omp_res += integral(X[i], X[i + 1], LeftY, RightX, StepX, StepY);
	}

	omp_end = omp_get_wtime();
	cout << "omp_res: " << omp_res << endl;
	cout << "omp_time: " << omp_end - omp_start << endl;
	cout << "threads: " << thread_num << endl;
	return 1;
}