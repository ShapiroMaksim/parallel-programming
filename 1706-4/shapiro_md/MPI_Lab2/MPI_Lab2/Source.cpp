#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "mpi.h"
#include "THeadList.h"

#define FORK_REQUEST 1
#define FORK_RESPONSE 2
#define FORK_RELEASE 3

using namespace std;

void Waiter(int _ProcNum, int _ProcRank);
void Philosopher(int _ProcNum, int _ProcRank);
void main(int argc, char* argv[]) {
	//setlocale(LC_ALL, "Russian");
	int ProcNum, ProcRank;
	MPI_Init(&argc, &argv);//Инициализация MPI
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	if (ProcRank == 0) {
		Waiter(ProcNum, ProcRank);//Функция официанта
	}
	else {
		Philosopher(ProcNum, ProcRank);//Функция философа
	}
	MPI_Finalize();
}

void Philosopher(int _ProcNum, int _ProcRank) {
	cout << "Philpsopher " << _ProcRank <<" is here" << endl;
	int in_buf = NULL, out_buf = NULL;//Буферы
	
	MPI_Status stat;
	Sleep(rand() % 10 * 1000);
	while (true) {
		MPI_Send(&out_buf, 1, MPI_INT, 0, FORK_REQUEST, MPI_COMM_WORLD);//Запрос вилок
		cout <<"Philosopher " << _ProcRank << " ask for forks" << endl;
		MPI_Recv(&in_buf, 1, MPI_INT, 0, FORK_RESPONSE, MPI_COMM_WORLD, &stat);//полчает разрешение взяять вилки
		cout << "Philosopher "<< _ProcRank << " is eating"<< endl;
		Sleep(rand() % 10 * 1000);
		MPI_Send(&out_buf, 1, MPI_INT, 0, FORK_RELEASE, MPI_COMM_WORLD);//освобождение вилок
		cout << "Philosopher "<< _ProcRank << " is thinking"<< endl;
	}
}

void Waiter(int _ProcNum, int _ProcRank) {
	cout << "Waiter "<< _ProcRank << " is here" << endl;
	int in_buf = NULL, out_buf = NULL, SourceNum;
	THeadList<int> queue;//очередь
	MPI_Status stat;
	bool* fork = new bool[_ProcNum - 1];//массив вилок
	for (int i = 0; i < _ProcNum - 1; i++) {
		fork[i] = true;
	}
	
	while (true) {
		MPI_Recv(&in_buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);//принимаем сообщения от философа
		SourceNum = stat.MPI_SOURCE;
		if (stat.MPI_TAG == FORK_REQUEST) {
			cout <<"Waiter got fork request from philosopher " << SourceNum << endl;
			if (fork[SourceNum % (_ProcNum - 1)] == true && fork[SourceNum - 1] == true) {//если обе вилки свободны
				fork[SourceNum % (_ProcNum - 1)] = false; //правая вилка занята
				fork[SourceNum - 1] = false; //левая вилка занята
				MPI_Send(&out_buf, 1, MPI_INT, SourceNum, FORK_RESPONSE, MPI_COMM_WORLD);//посылаем разрешение философу
				cout << "Waiter sent the forks to philosopher " << SourceNum << endl;
			}
			else { //если вилки не свободны
				//cout << "Waiter puts the philosopher " << SourceNum << " in line" << endl;
				if (!queue.IsEmpty()) {
					queue.InsLast(SourceNum); //помещаем философа в очередь
				}
				else {
					queue.InsFirst(SourceNum);
				}
			}
		}
		if (stat.MPI_TAG == FORK_RELEASE) {//сообщение об освобождении вилок
			fork[SourceNum % (_ProcNum - 1)] = true; //правая вилка свободна
			fork[SourceNum - 1] = true; //левая вилка свободна
			cout << "Waiter releases forks from philosopher " << SourceNum << endl;
			if (!queue.IsEmpty()) {//если есть ожидающие философы
				for (queue.Reset(); !queue.IsEnd(); queue.GoNext()) {
					SourceNum = queue.GetCurrVal();
					if (fork[SourceNum % (_ProcNum - 1)] == true && fork[SourceNum - 1] == true) {//если в очереди нащелся такой филосов который может взять две вилки
						fork[SourceNum % (_ProcNum - 1)] = false; //правая вилка занята
						fork[SourceNum - 1] = false; //левая вилка занята
						MPI_Send(&out_buf, 1, MPI_INT, SourceNum, FORK_RESPONSE, MPI_COMM_WORLD);//посылаем разрешение философу
						cout << "Waiter sent the forks to philosopher " << SourceNum << endl;
						queue.DelCurr();
					}
				}
			}
		}
	}
	
}