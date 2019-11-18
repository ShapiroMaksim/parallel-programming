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
	MPI_Init(&argc, &argv);//������������� MPI
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	if (ProcRank == 0) {
		Waiter(ProcNum, ProcRank);//������� ���������
	}
	else {
		Philosopher(ProcNum, ProcRank);//������� ��������
	}
	MPI_Finalize();
}

void Philosopher(int _ProcNum, int _ProcRank) {
	cout << "Philpsopher " << _ProcRank <<" is here" << endl;
	int in_buf = NULL, out_buf = NULL;//������
	
	MPI_Status stat;
	Sleep(rand() % 10 * 1000);
	while (true) {
		MPI_Send(&out_buf, 1, MPI_INT, 0, FORK_REQUEST, MPI_COMM_WORLD);//������ �����
		cout <<"Philosopher " << _ProcRank << " ask for forks" << endl;
		MPI_Recv(&in_buf, 1, MPI_INT, 0, FORK_RESPONSE, MPI_COMM_WORLD, &stat);//������� ���������� ������ �����
		cout << "Philosopher "<< _ProcRank << " is eating"<< endl;
		Sleep(rand() % 10 * 1000);
		MPI_Send(&out_buf, 1, MPI_INT, 0, FORK_RELEASE, MPI_COMM_WORLD);//������������ �����
		cout << "Philosopher "<< _ProcRank << " is thinking"<< endl;
	}
}

void Waiter(int _ProcNum, int _ProcRank) {
	cout << "Waiter "<< _ProcRank << " is here" << endl;
	int in_buf = NULL, out_buf = NULL, SourceNum;
	THeadList<int> queue;//�������
	MPI_Status stat;
	bool* fork = new bool[_ProcNum - 1];//������ �����
	for (int i = 0; i < _ProcNum - 1; i++) {
		fork[i] = true;
	}
	
	while (true) {
		MPI_Recv(&in_buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);//��������� ��������� �� ��������
		SourceNum = stat.MPI_SOURCE;
		if (stat.MPI_TAG == FORK_REQUEST) {
			cout <<"Waiter got fork request from philosopher " << SourceNum << endl;
			if (fork[SourceNum % (_ProcNum - 1)] == true && fork[SourceNum - 1] == true) {//���� ��� ����� ��������
				fork[SourceNum % (_ProcNum - 1)] = false; //������ ����� ������
				fork[SourceNum - 1] = false; //����� ����� ������
				MPI_Send(&out_buf, 1, MPI_INT, SourceNum, FORK_RESPONSE, MPI_COMM_WORLD);//�������� ���������� ��������
				cout << "Waiter sent the forks to philosopher " << SourceNum << endl;
			}
			else { //���� ����� �� ��������
				//cout << "Waiter puts the philosopher " << SourceNum << " in line" << endl;
				if (!queue.IsEmpty()) {
					queue.InsLast(SourceNum); //�������� �������� � �������
				}
				else {
					queue.InsFirst(SourceNum);
				}
			}
		}
		if (stat.MPI_TAG == FORK_RELEASE) {//��������� �� ������������ �����
			fork[SourceNum % (_ProcNum - 1)] = true; //������ ����� ��������
			fork[SourceNum - 1] = true; //����� ����� ��������
			cout << "Waiter releases forks from philosopher " << SourceNum << endl;
			if (!queue.IsEmpty()) {//���� ���� ��������� ��������
				for (queue.Reset(); !queue.IsEnd(); queue.GoNext()) {
					SourceNum = queue.GetCurrVal();
					if (fork[SourceNum % (_ProcNum - 1)] == true && fork[SourceNum - 1] == true) {//���� � ������� ������� ����� ������� ������� ����� ����� ��� �����
						fork[SourceNum % (_ProcNum - 1)] = false; //������ ����� ������
						fork[SourceNum - 1] = false; //����� ����� ������
						MPI_Send(&out_buf, 1, MPI_INT, SourceNum, FORK_RESPONSE, MPI_COMM_WORLD);//�������� ���������� ��������
						cout << "Waiter sent the forks to philosopher " << SourceNum << endl;
						queue.DelCurr();
					}
				}
			}
		}
	}
	
}