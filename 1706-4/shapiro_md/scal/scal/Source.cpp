
#include"stdio.h"
#include"stdlib.h"
#include"mpi.h"

void main(int argc, char* argv[]) {
	double* v1 = NULL, * v2 = NULL,/* * _v1, * _v2,*/* buf1 = NULL, * buf2 = NULL;
	int i, vector_size = 0, _vector_size, ProcNum, ProcRank;
	double sum = 0, sum_all = 0, st = 0, fn = 0, _st = 0, _fn = 0;
	
	/*scanf_s("%d", &vector_size);
	_v1 = new double[vector_size];
	_v2 = new double[vector_size];
	for (int j = 0; j < vector_size; j++) {
		_v1[j] = rand() % 10;
		_v2[j] = rand() % 10;
		printf("%10.2f%10.2f\n", _v1[j], _v2[j]);
	}
	st = MPI_Wtime();
	for (int j = 0; j < vector_size; j++) {
		sum += _v1[j] * _v2[j];
	}
	printf("Result = %10.2f\n", sum);
	printf("Linear time = %10.2f\n", (fn - st));
	*/
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	if (ProcRank == 0) {
		scanf_s("%d", &vector_size);
		v1 = new double[vector_size];
		v2 = new double[vector_size];
		buf1 = new double[vector_size];
		buf2 = new double[vector_size];


		for (int j = 0; j < vector_size; j++) {
			v1[j] = rand() % 10;
			v2[j] = rand() % 10;
			printf("%10.2f %10.2f\n", v1[j], v2[j]);
		}
		/*for (int j = 0; j < vector_size; j++) {
			v1[j] = _v1[j];
			v2[j] = _v2[j];
		}*/
		//линейный алгоритм
		st = MPI_Wtime();
		for (int j = 0; j < vector_size; j++) {
			sum += v1[j] * v2[j];
		}
		fn = MPI_Wtime();
		printf("Linear result = %10.2f\n", sum);
		printf("Linear time = %10.2f\n", (fn - st)*1000);
	}
	//параллельный алгоритм
	_st = MPI_Wtime();
	MPI_Bcast(&vector_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	_vector_size = vector_size / ProcNum;
	if (ProcRank != 0) {
		v1 = new double[_vector_size];
		v2 = new double[_vector_size];
		buf1 = new double[_vector_size];
		buf2 = new double[_vector_size];
	}
	MPI_Scatter(v1, _vector_size, MPI_DOUBLE, buf1, _vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(v2, _vector_size, MPI_DOUBLE, buf2, _vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	sum = sum_all = 0;
	for (i = 0; i < _vector_size; i++) {
		sum += buf1[i] * buf2[i];
	}
	MPI_Reduce(&sum, &sum_all, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	//_fn = MPI_Wtime();
	if (ProcRank == 0) {
		if (vector_size % ProcNum != 0)
			sum_all += v1[vector_size-1] * v2[vector_size-1];
		_fn = MPI_Wtime();
		printf("Parallel result = %10.2f\n", sum_all);
		printf("Parallel time = %10.2f\n", (_fn - _st)*1000);
	}
	MPI_Finalize();
	delete[] v1;
	delete[] v2;
}