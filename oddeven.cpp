#include <iostream>
#include <time.h>
#include <stdlib.h>
#include<algorithm>
#include<omp.h>
#include<vector>
using namespace std;

int numThreads = 4;
vector<int> a;
vector<int> b;
vector<int> c;
int lenArr;

void oddEvenMerge(int lo, int n, int r)
{
    int m=r*2;
    if (m<n)
    {
        oddEvenMerge(lo, n, m);
        oddEvenMerge(lo+r, n, m);
        for (int i=lo+r; i+r<lo+n; i+=m)
        {
        	if (a[i]>a[i+r])
            {
            	int t=a[i];
		        a[i]=a[i+r];
		        a[i+r]=t;
            }
        }
    }
    else
    	if (a[lo]>a[lo+r])
           {
           	int t=a[lo];
		    a[lo]=a[lo+r];
		    a[lo+r]=t;
        }
}

void oddEvenMergeSort(int lo, int n)
{
    if (n>1)
    {
        int m=n/2;
        oddEvenMergeSort(lo, m);
        oddEvenMergeSort(lo+m, m);
        oddEvenMerge(lo, n, 1);
    }
}

void sortSerial()
{
    oddEvenMergeSort(0, a.size());
}

void sortParallel(int t)
{
	if(t==2){
		 #pragma omp parallel sections num_threads(2)
         {
         	#pragma omp section
        		oddEvenMergeSort(0, a.size()/2);
        	 #pragma omp section
                oddEvenMergeSort(a.size()/2, a.size()/2);

         }
        oddEvenMerge(0, a.size(), 1);
	}
	else if(t==4){
		#pragma omp parallel sections num_threads(4)
         {
         	#pragma omp section
        		oddEvenMergeSort(0, a.size()/4);
        	 #pragma omp section
                oddEvenMergeSort(a.size()/4, a.size()/4);
			#pragma omp section
        		oddEvenMergeSort(a.size()/2, a.size()/4);
        	 #pragma omp section
                oddEvenMergeSort(a.size()/2 + a.size()/4, a.size()/4);
         }
         #pragma omp parallel sections num_threads(2)
		 {
         	#pragma omp section
         	oddEvenMerge(0,a.size()/2,1);
         	#pragma omp section
         	oddEvenMerge(a.size()/2,a.size()/2,1);
		 }
        oddEvenMerge(0, a.size(), 1);
	}
	else{
		oddEvenMergeSort(0,a.size());
	}
}

bool checkResult(vector<int> &A,vector<int> &B){
	int i = 0;
	int lenArr=A.size();
	while(i<lenArr)
	{
		if(A[i]!=B[i]) { return false; }
		i++;
	}
	return true;
}

void printArray(vector<int> &A){
	int lenArr = A.size();
	if( lenArr <= 30 ){
		for(int i = 0 ; i < lenArr; i++ ) 
		{
			printf("%d ", A[i]);
		}
		printf("\n");
	}
}

vector<int> addPadding(vector<int> &A){
	int lenArr2 = 1;
	while(lenArr2 < lenArr)
		lenArr2*=2;
	vector<int> B(lenArr2);
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

vector<int> removePadding(vector<int> &A,int originalLength){
	int r = lenArr-originalLength;
	vector<int> B(originalLength);
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
	double startTime,stopTime;
	vector<int> temp;
	srand(time(NULL));
	cout<<"Number of Threads are-"<<numThreads<<endl;
    cout<<"enter the number of elements to be sorted (number preffered in the order of 2^n)";     
    cin>>lenArr;
    a.resize(lenArr);
    b.resize(lenArr);
    c.resize(lenArr);
	int originalLength=lenArr;
	printf("Initializing the arrays with random numbers...\n");
	for (int i = 0; i < lenArr; i++){
		a.push_back((rand() % 50000));
		b.push_back(a[i]);
		c.push_back(a[i]);
	}
	
	printf("Initialization complete\n");
	printf("\n\nArray BEFORE sorting: \n");
	printArray(a);
	
	a=addPadding(a);
	lenArr=originalLength;
	b=addPadding(b);
	lenArr=originalLength;
	c=addPadding(c);
	
	printf("\nSorting with serial sort function of 'algorithm.h' ...");
	startTime = clock();
	sort(c.begin(),c.end());
	stopTime = clock();
	c=removePadding(c,originalLength);
	printArray(c);
	printf("Sorted in (aprox.): %f seconds \n\n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);
	
    printf("\nSorting with custom serial OddEvenMergeSortSerial...");	
	startTime = clock();
	sortSerial();
	stopTime = clock();
	a=removePadding(a,originalLength);
	printArray(a);
    printf("Sorted in (aprox.): %f seconds \n",(double)(stopTime-startTime)/CLOCKS_PER_SEC);
    
	printf("\nSorting with custom parallel OddEvenMergeSortParallel...");	
	startTime = clock();
	temp=a;
	a=b;
	sortParallel(numThreads);
	stopTime = clock();
	a=removePadding(a,originalLength);
	printArray(a);
	b=a;
	a=temp;
    printf("Sorted in (approx.): %f seconds \n",(double)(stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nChecking if the results are correct...\n");
	bool correctResult;
	correctResult= checkResult(a,c);
	if(correctResult){
		printf("The result with 'custom SERIAL OddEvenMergeSortSerial' is CORRECT\n");
	}else{
		printf("The result with 'custom SERIAL OddEvenMergeSortSerial' is INCORRECT!!\n");
	}

	correctResult = checkResult(b,c);
	if(correctResult==true){
		printf("The result with 'custom PARALLEL OddEvenMergeSortParallel' is CORRECT\n\n");
	}else{
		printf("The result with 'custom PARALLEL OddEvenMergeSortParallel' is INCORRECT!!\n");
	}
	
	return 0;
}
