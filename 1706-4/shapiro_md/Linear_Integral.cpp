#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <string>

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
	
	double res;
	if (argc < 7) {
		return -1;
	}
	LeftX = stof(argv[1]);
	RightX = stof(argv[2]);
	LeftY = stof(argv[3]);
	RightY = stof(argv[4]);
	StepX = stof(argv[5]);
	StepY = stof(argv[6]);
	clock_t start = clock();
	res = integral(LeftX, RightX, LeftY, RightY, StepX, StepY);
	clock_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "res: " << res << endl;
	cout << "time: " << time << endl;
	


	return 1;
}