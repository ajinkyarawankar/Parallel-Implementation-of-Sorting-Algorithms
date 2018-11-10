#include<iostream>                                                                             
#include<omp.h>           
#include<time.h>
#include<cstdlib>
#include <algorithm>
using namespace std;

int numThreads = 2;
int lenArr;

void Swap(int i, int j, int *a, int dir)                                         
{	
	if (dir==(a[i]>a[j]))
    {
    	int t=a[i];
        a[i]=a[j];
        a[j]=t;
    }
}

void bitonicMerge(int startIndex, int lastIndex, int dir, int *ar)    
{
		int counter = 0;                                                                   
		int noOfElements = lastIndex - startIndex + 1;
		for (int j = noOfElements / 2; j > 0; j = j / 2)
		{
			counter = 0;
			for (int i = startIndex; i + j <= lastIndex; i++)
			{
					Swap(i, i + j, ar,dir);
					counter++;
			}
		}
}
void bitonicSortParallel(int startIndex, int lastIndex, int *ar)                         
{
	int noOfElements = lastIndex - startIndex + 1;
	for (int j = 2; j <= noOfElements; j = j * 2)
	{
		#pragma omp parallel for                                                      
		for (int i = 0; i < noOfElements; i = i + j)
		{
			if (((i / j) % 2) == 0)                                                              
			{
				bitonicMerge(i, i + j - 1, 1, ar);
			}
			else
			{
				bitonicMerge(i, i + j - 1, 0, ar);
			}
		}
	}
}

void bitonicSortSerial(int startIndex, int lastIndex, int *ar)                        
{
	int noOfElements = lastIndex - startIndex + 1;
	for (int j = 2; j <= noOfElements; j = j * 2)
	{
		for (int i = 0; i < noOfElements; i = i + j)
		{
			if (((i / j) % 2) == 0)                                                              
			{
				bitonicMerge(i, i + j - 1, 1, ar);
			}
			else
			{
				bitonicMerge(i, i + j - 1, 0, ar);
			}
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

int* addPadding(int *A){
	int lenArr2 = 1;
	while(lenArr2 < lenArr)
	    lenArr2*=2;
	int *B = new int[lenArr2];
	int i=0;
	while(i<lenArr)
	{
		B[i]=A[i];
		i++;
	}
	while(i<lenArr2)
	{
		B[i]=0;
		i++;
	}
	lenArr=lenArr2;
	return B;
}

int* removePadding(int *A,int originalLength){
	int r = lenArr-originalLength;
	int *B = new int[originalLength];
	int i=0;
	while(r<lenArr){
		B[i]=A[r];
		r++;
		i++;
	}
	return B;
}

int main()                                                                                      
{
	omp_set_num_threads(numThreads);
	cout<<"Number of Threads are-"<<numThreads<<endl;	                                                   
	cout << "enter the number of elements to be sorted (number preffered in the order of 2^n)";    
	cin >> lenArr;
	double startTime, stopTime;
	int *A = new int[lenArr];
	int *B = new int[lenArr];
	int *C = new int[lenArr];
	srand(time(NULL));
	int i;
	printf("Initializing the arrays with random numbers...\n");
	for (i=0; i<lenArr; i++){
		A[i] = (rand()%1000000);
		B[i] = A[i];
		C[i] = A[i];
	}
	printf("Initialization complete\n");
	printArray(A,lenArr);
	int originalLength = lenArr;
	A=addPadding(A);
	lenArr=originalLength;
	B=addPadding(B);
	lenArr=originalLength;
	C=addPadding(C);
	
	printf("\nSorting with serial sort function of 'algorithm.h' ...");
	startTime = clock();
	sort(A,A+lenArr);
	stopTime = clock();
	A=removePadding(A,originalLength);
	printArray(A,originalLength);
	printf("Sorted in (aprox.): %f seconds \n\n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nSorting with custom serial BitonicSortSerial...");
	startTime = clock();
	bitonicSortSerial(0,lenArr-1,B);
	stopTime = clock();
	B=removePadding(B,originalLength);
	printArray(B,originalLength);
	printf("Sorted in (aprox.): %f seconds \n\n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nSorting with custom PARALLEL BitonicSortParallel... "); fflush(stdout);
	startTime = clock();
	bitonicSortParallel(0,lenArr-1,C);
	stopTime = clock();
	C=removePadding(C,originalLength);
	printArray(C,originalLength);
	printf("Sorted in (aprox.): %f seconds \n\n", (stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nChecking if the results are correct...\n");
	bool correctResult;
	correctResult = checkResult(A,B,originalLength);
	if(correctResult==true){
		printf("The result with 'custom SERIAL BitonicSortSerial' is CORRECT\n");
	}else{
		printf("The result with 'custom SERIAL BitonicSortSerial' is INCORRECT!!\n");
	}

	correctResult = checkResult(A,C,originalLength);
	if(correctResult==true){
		printf("The result with 'custom PARALLEL BitonicSortParallel' is CORRECT\n\n");
	}else{
		printf("The result with 'custom PARALLEL BitonicSortParallel' is INCORRECT!!\n");
	}

	free(A);
	free(C);
	free(B);
}

