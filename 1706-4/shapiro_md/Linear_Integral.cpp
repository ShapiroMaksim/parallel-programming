#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;


double integral(const double LeftX, const double RightX, const double LeftY,
	const double RightY, const double StepX, const double StepY/*, double* res*/)
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
				/ 4.0; /*(i * j + (i + _Step) * j + i * (j + _Step) + (i + _Step)*(j + _Step)) / 4;*/
			//midval = (i + _Step / 2) * (j + _Step / 2) * sin((i + _Step / 2) * (j + _Step / 2));
			sum += midval * StepX * StepY;
			
		}
	}
	return sum;
	
}


int main(int argc, char* argv[])
{
	double LeftX, RightX, LeftY, RightY, StepX, StepY;
	/*double LeftX = 0.0;
	double RightX = 10.0;
	double LeftY = 0.0;
	double RightY = 10.0;
	double StepX = 0.001;
	double StepY = 0.001;*/
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
	
	res = integral(LeftX, RightX, LeftY, RightY, StepX, StepY);	
	
	cout << "res: " << res;
	//cout << "linear res: " << Linear_res << "\nlinear time: " << End_timeL - Start_timeL << "\n";
	

	return 1;
}