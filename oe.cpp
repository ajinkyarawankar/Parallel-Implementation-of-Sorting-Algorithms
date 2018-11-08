#include<iostream>
#include<vector>
#include<omp.h>
#include<string>
#include<time.h>
#include<cstdlib>
using namespace std;

void OddEvenSort( int *A, int N) 
{
 int exch0 , exch1 = 1 , trips = 0 , i ;
while ( exch1 ) 
{
 exch0 = 0;
  exch1 = 0;
#pragma omp parallel num_threads(4)
 {
  int temp ;
   #pragma omp for 
   for ( i = 0; i < N-1; i += 2)
    {
	 if (A[ i ] > A[ i +1]) 
	 { 
	 temp = A[ i ] ; 
	 A[ i ] = A[ i +1]; 
	 A[ i +1] = temp ; 
	 exch0 = 1;
	  }
	   }
if ( exch0 || !trips )
 {
  #pragma omp for 
  for ( i = 1; i < N-1; i += 2)
   { 
   if (A[ i ] > A[ i +1]) 
   { 
   temp = A[ i ] ; 
   A[ i ] = A[ i +1]; 
   A[ i +1] = temp ;
    exch1 = 1; 
	} 
	}
	 } // i f exch0 
	 } // end p a r a l l e l 
	 trips = 1;
	  }
}


void OddEvenSortS( int *A, int N) 
{
 int exch0 , exch1 = 1 , trips = 0 , i ;
while ( exch1 ) 
{
 exch0 = 0;
  exch1 = 0;
//#pragma omp parallel
// {
  int temp ;
//   #pragma omp for 
   for ( i = 0; i < N-1; i += 2)
    {
	 if (A[ i ] > A[ i +1]) 
	 { 
	 temp = A[ i ] ; 
	 A[ i ] = A[ i +1]; 
	 A[ i +1] = temp ; 
	 exch0 = 1;
	  }
	   }
if ( exch0 || !trips )
 {
//  #pragma omp for 
  for ( i = 1; i < N-1; i += 2)
   { 
   if (A[ i ] > A[ i +1]) 
   { 
   temp = A[ i ] ; 
   A[ i ] = A[ i +1]; 
   A[ i +1] = temp ;
    exch1 = 1; 
	} 
	}
	 } // i f exch0 
//	 } // end p a r a l l e l 
	 trips = 1;
	  }
}


int main(){
	
	
	omp_set_num_threads(4);                                                     //set the no of threads
	int N;
	std::cout << "enter the number of elements to be sorted (number should be in the order of 2^n)";     //try to fix this issue 
	cin >> N;
	int *A = new int[N];
	int *B = new int[N];
	srand(time(NULL));

	for (int i = 0; i < N; i++) {
		A[i] = B[i] = (rand() % 10000);
	}
	
	
	clock_t t;
	t = clock();
	OddEvenSort(A,N);
	t = clock() - t;
	printf("It parallel sort %d clicks %f seconds \n", t, ((float)t) / CLOCKS_PER_SEC);
	
//	for (int i = 0; i < N; i++) {
//		cout<<A[i]<<endl;
//	}
	
	clock_t t1;
	t1 = clock();
	OddEvenSortS(B,N);
	t1 = clock() - t1;
	printf("It serial sort %d clicks %f seconds \n", t1, ((float)t1) / CLOCKS_PER_SEC);
	
	
	
}
