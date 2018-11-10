#include <iostream>
#include <time.h>
#include <stdlib.h>
#include<algorithm>
#include<omp.h>
using namespace std;

int numThreads = 4;

void merge(int *A, int *temp, int from, int mid, int to,int lenArr)
{
	int k = from, i = from, j = mid + 1;
	while (i <= mid && j <= to)
	{
		if (A[i] < A[j])
			temp[k++] = A[i++];
		else
			temp[k++] = A[j++];
	}
	while (i < lenArr && i <= mid)
		temp[k++] = A[i++];
	for (int i = from; i <= to; i++)
		A[i] = temp[i];
}

void mergeSortSerial(int *A, int *temp, int low, int high,int lenArr)
{
	for (int size = 1; size <= high - low; size = 2*size)
	{
		for (int i = low; i < high; i += 2*size)
		{
			int from = i;
			int mid = i + size - 1;
			int to = min(i + 2*size - 1, high);
			merge(A, temp, from, mid, to,lenArr);
		}
	}
}

void mergeSortParallel(int *A ,int *temp, int low, int high,int lenArr)
{ 
	for (int size = 1; size <= high - low; size = 2*size)
	{
		#pragma omp parallel for num_threads(numThreads)
		for (int i = low; i < high; i += 2*size)
		{
			int from = i;
			int mid = i + size - 1;
			int to = min(i + 2*size - 1, high);
			merge(A, temp, from, mid, to,lenArr);
		}
	}
}

bool checkResult(int *A,int *B,int lenArr){
	int i = 0;
	while(i<lenArr)
	{
		if(A[i]!=B[i]) { return false; }
		i++;
	}
	return true;
}

void printArray(int *A,int lenArr){
	if( lenArr <= 30 ){
		for(int i = 0 ; i < lenArr; i++ ) 
		{
			printf("%d ", A[i]);
		}
		printf("\n");
	}
}

int main()
{
	omp_set_num_threads(numThreads);
	int lenArr;
	double startTime,stopTime;
	cout<<"Number of Threads are-"<<numThreads<<endl;
	cout<<"enter the number of elements to be sorted";     
    cin>>lenArr;
	
	int *C = new int[lenArr];
	int *A = new int[lenArr];
	int *B = new int[lenArr];
	int *temp = new int[lenArr];
	int *temp2 = new int[lenArr];
	srand(time(NULL));
//............................................................................................//
	printf("Initializing the arrays with random numbers...\n");
	for (int i = 0; i < lenArr; i++){
		temp[i] = A[i] = B[i] = C[i] = temp2[i] = (rand() % 5000000);
	}
	printf("Initialization complete\n");
	printf("\n\nArray BEFORE sorting: \n");
	printArray(A,lenArr);

//............................................................................................//
	printf("\nSorting with serial sort function of 'algorithm.h' ...");
	startTime = clock();
	sort(C,C+lenArr);
	stopTime = clock();
	printArray(C,lenArr);
	printf("Sorted in (aprox.): %f seconds \n\n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);

//............................................................................................//

	printf("\nSorting with custom serial MergeSortSerial...");	
	startTime = clock();
	mergeSortSerial(A, temp, 0, lenArr - 1,lenArr);
	stopTime = clock();
	printArray(A,lenArr);
    printf("Sorted in (aprox.): %f seconds \n",(double)(stopTime-startTime)/CLOCKS_PER_SEC);

//.............................................................................................//

	printf("\nSorting with custom parallel MergeSortParallel...");	
	startTime = clock();
	mergeSortParallel(B, temp2, 0, lenArr - 1,lenArr);
	stopTime = clock();
	printArray(B,lenArr);
    printf("Sorted in (approx.): %f seconds \n",(double)(stopTime-startTime)/CLOCKS_PER_SEC);

//.............................................................................................//

	printf("\nChecking if the results are correct...\n");
	bool correctResult = checkResult(A,C,lenArr);
	if(correctResult){
		printf("The result with 'custom SERIAL MergeSortSerial' is CORRECT\n");
	}else{
		printf("The result with 'custom SERIAL MergeSortSerial' is INCORRECT!!\n");
	}

	correctResult = checkResult(B,C,lenArr);
	if(correctResult==true){
		printf("The result with 'custom PARALLEL MergeSortParallel' is CORRECT\n\n");
	}else{
		printf("The result with 'custom PARALLEL MergeSortParallel' is INCORRECT!!\n");
	}
	return 0;
}
