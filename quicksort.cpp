#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <iostream>
#include <algorithm>
using namespace std;

int numThreads = 4;

int partition (int *arr, int low, int high) 
{ 
    int pivot = arr[high];  
    int i = (low - 1); 
  	int j,t;
    for (j = low; j <= high- 1; j++) 
    { 
        if (arr[j] <= pivot) 
        { 
            i++;    
            t = arr[i];
            arr[i] = arr[j];
            arr[j]= t;
        } 
    } 
	t = arr[i+1];
    arr[i+1] = arr[high];
    arr[high]= t;
    return (i + 1); 
} 

void quickSortSerialParallel_internal(int* array, int left, int right, int cutoff) 
{
	if(left<right){
		int i = left, j = right;
	int tmp;
	int pivot = partition(array,left,right);


	if ( ((right-left)<cutoff) ){
		quickSortSerialParallel_internal(array, left, pivot-1, cutoff); 			
		quickSortSerialParallel_internal(array, pivot+1, right, cutoff); 

	}else{
		#pragma omp task 	
		 quickSortSerialParallel_internal(array, left, pivot-1, cutoff); 
		#pragma omp task 	
		 quickSortSerialParallel_internal(array, pivot+1, right, cutoff); 	
	}
	
	}
}

void quickSortSerialParallel(int* array, int lenArray, int numThreads){

	int cutoff = 1000;

	#pragma omp parallel num_threads(numThreads)
	{	
		#pragma omp single nowait
		{
			quickSortSerialParallel_internal(array, 0, lenArray-1, cutoff);	
		}
	}	

}

void quickSortSerial(int* arr, int left, int right) 
{
	if(left<right){
	int i = left, j = right;
	int tmp;
	int pivot = partition(arr,left,right);	
		quickSortSerial(arr, left, pivot-1);
		quickSortSerial(arr, pivot+1, right);
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

int main(){
	int lenArr;
	cout<<"Number of Threads are-"<<numThreads<<endl;
	cout<<"enter the number of elements to be sorted";     
    cin>>lenArr;
	double startTime, stopTime;

	int* arr1;
	int* arr2; 
	int* arr3;
	arr1 = (int*) malloc(lenArr*sizeof(int));
	arr2 = (int*) malloc(lenArr*sizeof(int));
	arr3 = (int*) malloc(lenArr*sizeof(int));

	int i;
	srand(5);
	printf("Initializing the arrays with random numbers...\n");
	for (i=0; i<lenArr; i++){
		arr1[i] = rand()%5000000;
		arr2[i] = arr1[i];
		arr3[i] = arr1[i];
	}
	printf("Initialization complete\n");
	printArray(arr1,lenArr);
	
	printf("\nSorting with serial sort function of 'algorithm.h' ...");
	startTime = clock();
	sort(arr1,arr1+lenArr);
	stopTime = clock();
	printArray(arr1,lenArr);
	printf("Sorted in (aprox.): %f seconds \n\n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nSorting with custom serial quickSortSerial...");
	startTime = clock();
	quickSortSerial(arr2, 0, lenArr-1);
	stopTime = clock();
	printArray(arr2,lenArr);
	printf("Sorted in (aprox.): %f seconds \n\n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nSorting with custom PARALLEL quickSortParallel... "); fflush(stdout);
	startTime = clock();
	quickSortSerialParallel(arr3, lenArr, numThreads);
	stopTime = clock();
	printArray(arr3,lenArr);
	printf("Sorted in (aprox.): %f seconds \n\n", (stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nChecking if the results are correct...\n");
	bool correctResult;
	correctResult = checkResult(arr1,arr2,lenArr);
	if(correctResult==true){
		printf("The result with 'custom SERIAL quickSortSerial' is CORRECT\n");
	}else{
		printf("The result with 'custom SERIAL quickSortSerial' is INCORRECT!!\n");
	}

	correctResult = checkResult(arr1,arr3,lenArr);
	if(correctResult==true){
		printf("The result with 'custom PARALLEL quickSortParallel' is CORRECT\n\n");
	}else{
		printf("The result with 'custom PARALLEL quickSortParallel' is INCORRECT!!\n");
	}

	free(arr1);
	free(arr2);
	free(arr3);

	return 0;
}
