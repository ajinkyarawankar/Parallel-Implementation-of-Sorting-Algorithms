#include<iostream>
#include<omp.h>
#include<time.h>
using namespace std;

int N;

void quicksortS(int Array[],int first,int last){ 
    int pivot,j,temp,i; 
 
     if(first<last){ 
         pivot=first; 
         i=first; 
         j=last; 
 
         while(i<j){ 
             
            while(Array[i]<=Array[pivot]&&i<last) 
                 i++; 
                while(Array[j]>Array[pivot]) 
                    j--; 
                if(i<j){ 
                     temp=Array[i]; 
                      Array[i]=Array[j]; 
                      Array[j]=temp; 
                } 
        } 
 
         temp=Array[pivot]; 
         Array[pivot]=Array[j]; 
         Array[j]=temp; 
 
         quicksortS(Array,first,j-1); 
         quicksortS(Array,j+1,last); 
    }
}  

void quicksortP(int Array[],int first,int last){ 
    int pivot,j,temp,i; 
 
     if(first<last){ 
         pivot=first; 
         i=first; 
         j=last; 
 
         while(i<j){ 
             
            while(Array[i]<=Array[pivot]&&i<last) 
                 i++; 
                while(Array[j]>Array[pivot]) 
                    j--; 
                if(i<j){ 
                     temp=Array[i]; 
                      Array[i]=Array[j]; 
                      Array[j]=temp; 
                } 
        } 
 
         temp=Array[pivot]; 
         Array[pivot]=Array[j]; 
         Array[j]=temp; 
            
        #pragma omp parallel sections
         {
          #pragma omp section
            quicksortP(Array,first,j-1); 
          #pragma omp section  
            quicksortP(Array,j+1,last); 
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
    omp_set_num_threads(2);

    cout<<"enter the number of elements to be sorted (number should be in the order of 2^n)";     //try to fix this issue 
    cin>>N;


    int A[N],B[N];
    srand(time(NULL));

    for (int i = 0; i < N; i++){
        A[i] = B[i]  = (rand() % 4096);

    }

    // printf("Original Array : ");
    // printArray(A);

    clock_t t;
    t=clock();
    
    quicksortP(B, 0, N - 1);
    t=clock()-t;
    printf("It parallel sort %d clicks %f seconds \n",t,((float)t)/CLOCKS_PER_SEC);

    clock_t t1;
    t1=clock();
    quicksortS(A,0, N - 1);
    t1=clock()-t1;
    printf("It serial sort %d clicks %f seconds \n",t1,((float)t1)/CLOCKS_PER_SEC);

    // printf("Modified Array : ");
    // printArray(A);
    // printf("Modified Array : ");
    // printArray(B);

    return 0;
}