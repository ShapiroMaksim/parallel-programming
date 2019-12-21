#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <math.h>

using namespace std;


void integral(const double _LeftX, const double _RightX, const double _LeftY,
	const double _RightY, const double _Step, double* res)
{	
	//   x*y*sin(x*y)
	
	double sum = 0.0;       
	double midval;

	for (double i = _LeftX; i < _RightX; i += _Step)
	{
		for (double j = _LeftY; j < _RightY; j += _Step)
		{
			midval = ((i * j * sin(i * j)) +
				((i+_Step) * j * sin((i+_Step) * j)) +
				(i * (j+_Step) * sin(i * (j+_Step))) +
				((i+_Step) * (j+_Step) * sin((i+_Step) * (j+_Step))))
				/ 4; /*(i * j + (i + _Step) * j + i * (j + _Step) + (i + _Step)*(j + _Step)) / 4;*/
			sum += midval * _Step * _Step;
		}
	}
	*res = sum;
}


void main(int argc, char* argv[])
{
	int procRank, procNum;
	
	double LeftX = 0.0;   
	double RightX = 10.0;   
	double LeftY = 0.0;    
	double RightY = 10.0; 
	double Step = 0.001;   
	double res;
	double Final_res = 0.0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
	double Start_time, End_time;
	Start_time = MPI_Wtime();
	double* X = NULL;

	if (procRank == 0)
	{
		X = new double[2 * procNum];
		double tmp = (RightX - LeftX) / (double)procNum;

		tmp = round(tmp * 100) / 100;
		X[0] = LeftX;
		X[1] = LeftX + tmp;

		if (procNum > 1) {
			for (int i = 2; i < procNum * 2 - 2; i += 2)
			{
				X[i] = X[i - 1];
				X[i + 1] = X[i] + tmp;
			}
			X[procNum * 2 - 2] = X[procNum * 2 - 3];
			X[procNum * 2 - 1] = RightX;
		}

	}
	
	double Segment[2];
	MPI_Scatter(X, 2, MPI_DOUBLE, Segment, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	integral(Segment[0], Segment[1], LeftY, RightY, Step, &res);

	MPI_Reduce(&res, &Final_res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (procRank == 0)
	{
		End_time = MPI_Wtime();
		cout << "res: " << Final_res << "\ntime: " << End_time-Start_time << "\nproc num: "<< procNum << "\n";	
	}

	MPI_Finalize();
}