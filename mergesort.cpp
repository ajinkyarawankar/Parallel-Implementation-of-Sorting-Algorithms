#include <iostream>
#include <time.h>
#include <stdlib.h>
#include<algorithm>
#include<omp.h>
#include<cstdlib>
using namespace std;

#define N 65536


void merge(int A[], int temp[], int from, int mid, int to)
{
	int k = from, i = from, j = mid + 1;
	while (i <= mid && j <= to)
	{
		if (A[i] < A[j])
			temp[k++] = A[i++];
		else
			temp[k++] = A[j++];
	}
	while (i < N && i <= mid)
		temp[k++] = A[i++];
	for (int i = from; i <= to; i++)
		A[i] = temp[i];
}

void mergesortS(int A[], int temp[], int low, int high)
{
	for (int tupleSize = 1; tupleSize <= high - low; tupleSize = 2*tupleSize)
	{
		for (int i = low; i < high; i += 2*tupleSize)
		{
			int from = i;
			int mid = i + tupleSize - 1;
			int to = min(i + 2*tupleSize - 1, high);
			merge(A, temp, from, mid, to);
		}
	}
}

void mergesortP(int A[], int temp[], int low, int high)
{ 
	for (int tupleSize = 1; tupleSize <= high - low; tupleSize = 2*tupleSize)
	{
		#pragma omp parallel for schedule(auto)
		for (int i = low; i < high; i += 2*tupleSize)
		{
			int from = i;
			int mid = i + tupleSize - 1;
			int to = min(i + 2*tupleSize - 1, high);
			merge(A, temp, from, mid, to);
		}
	}
}

int printArray(int A[])
{
	for (int i = 0; i < N; i++) {
		printf("%d ", A[i]);
	}

	printf("\n");
}

int main()
{
	omp_set_num_threads(4);
	int A[N],B[N], temp[N],temp2[N];
	srand(time(NULL));

	for (int i = 0; i < N; i++){
		temp[i] = A[i] = B[i] = temp2[i] = (rand() % 50);

	}

	// printf("Original Array : ");
	// printArray(A);

    clock_t t;
    t=clock();
	mergesortS(A, temp, 0, N - 1);
	t=clock()-t;
    printf("It serial sort %d clicks %f seconds \n",t,((float)t)/CLOCKS_PER_SEC);

    clock_t t1;
    t1=clock();
	mergesortP(B, temp2, 0, N - 1);
	t1=clock()-t1;
    printf("It parallel sort %d clicks %f seconds \n",t1,((float)t1)/CLOCKS_PER_SEC);

	// printf("Modified Array : ");
	// printArray(A);
	// printf("Modified Array : ");
	// printArray(B);

	return 0;
}
