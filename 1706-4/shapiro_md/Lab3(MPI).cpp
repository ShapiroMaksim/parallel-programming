#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <string>
using namespace std;


double integral(const double _LeftX, const double _RightX, const double _LeftY,
	const double _RightY, const double _StepX, const double _StepY)
{
	//   x*y*sin(x*y)

	double sum = 0.0;
	double midval;

	for (double i = _LeftX; i < _RightX; i += _StepX)
	{
		for (double j = _LeftY; j < _RightY; j += _StepY)
		{

			midval = ((i * j * sin(i * j)) +
				((i + _StepX) * j * sin((i + _StepX) * j)) +
				(i * (j + _StepY) * sin(i * (j + _StepY))) +
				((i + _StepX) * (j + _StepY) * sin((i + _StepX) * (j + _StepY))))
				/ 4.0; 
			sum += midval * _StepX * _StepY;

		}
	}
	return sum;

}


int main(int argc, char* argv[])
{
	int procRank, procNum;
	double LeftX, RightX, LeftY, RightY, StepX, StepY, Linear_res, Final_res;
	double res;
	if (argc < 6) {
		//throw -1;
		return -1;
	}
	LeftX = stof(argv[1]);
	RightX = stof(argv[2]);
	LeftY = stof(argv[3]);
	RightY = stof(argv[4]);
	StepX = stof(argv[5]);
	StepY = stof(argv[6]);
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
	double Start_time, End_time, Start_timeL, End_timeL;
	//Start_time = MPI_Wtime();
	double* X = NULL;

	if (procRank == 0)
	{   ///////////////// ËÈÍÅÉÍÛÉ ÀËÃÎÐÈÒÌ
		Start_timeL = MPI_Wtime();
		Linear_res = integral(LeftX, RightX, LeftY, RightY, StepX, StepY);
		End_timeL = MPI_Wtime();
		///////////////// ÏÀÐÀËËÅËÜÍÛÉ ÀËÃÎÐÈÒÌ
		Start_time = MPI_Wtime();
		X = new double[2 * procNum];
		double tmp = (RightX - LeftX) / (double)procNum;

		for (int i = 0; i < procNum * 2; i += 2) {
			X[i] = LeftX + i * tmp / 2;
			X[i + 1] = X[i] + tmp;
		}

	}

	double Segment[2];
	MPI_Scatter(X, 2, MPI_DOUBLE, Segment, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	res = integral(Segment[0], Segment[1], LeftY, RightY, StepX, StepY);

	MPI_Reduce(&res, &Final_res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (procRank == 0)
	{
		End_time = MPI_Wtime();
		cout << "res: " << Final_res << "\ntime: " << End_time - Start_time << "\nproc num: " << procNum << "\n";
		cout << "linear res: " << Linear_res << "\nlinear time: " << End_timeL - Start_timeL << "\n";
	}

	MPI_Finalize();
	delete[] X;
	return 1;
}