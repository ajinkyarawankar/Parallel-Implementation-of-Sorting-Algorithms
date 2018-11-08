#include <iostream>
#include <time.h>
#include <omp.h>
#include<cstdlib>
using namespace std; 

int N;

void bubbleSortS(int arr[]) 
{ 
   int i, j; 
   for (i = 0; i < N-1; i++)       
    {
      int first = i % 2;
        for (j = first; j < N-1; j+=2)  
               if (arr[j] > arr[j+1]) 
                  {
                    int t = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = t;
                  }
    }
} 

void bubbleSortP(int arr[]) 
{ 
   int i, j; 
   for (i = 0; i < N-1; i++)      
   {
    if(i%2==0){
    	#pragma omp parallel for 
        for (j = 0; j < N-1; j+=2)  
               if (arr[j] > arr[j+1]) 
                  {
                    int t = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = t;
                  }      
	}
	else{
		#pragma omp parallel for 
        for (j = 1; j < N-1; j+=2)  
               if (arr[j] > arr[j+1]) 
                  {
                    int t = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = t;
                  }      
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

    cout<<"enter the number of elements to be sorted (number should be in the order of 2^n)";     //try to fix this issue 
    cin>>N;

    int A[N],B[N];
    srand(time(NULL));

    for (int i = 0; i < N; i++){
        A[i] = B[i]  = (rand() % 100000);

    }

    // printf("Original Array : ");
    // printArray(A);

    clock_t t;
    t=clock();
    bubbleSortS(A); 
    t=clock()-t;
    printf("It serial sort %d clicks %f seconds \n",t,((float)t)/CLOCKS_PER_SEC);

    clock_t t1;
    t1=clock();
    bubbleSortP(B); 
    t1=clock()-t1;
    printf("It parallel sort %d clicks %f seconds \n",t1,((float)t1)/CLOCKS_PER_SEC);

//     printf("Sorted array : "); 
//     printArray(A);
//     printf("Sorted array : "); 
//     printArray(B);  
    return 0; 
} 
