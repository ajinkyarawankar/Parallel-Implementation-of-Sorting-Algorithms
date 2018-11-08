#include <iostream>
#include <time.h>
#include <stdlib.h>
#include<algorithm>
#include<omp.h>
#include<vector>
#include<cstdlib>
using namespace std;

int N;
     vector<int> a;
     vector<int> b;


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


    void oddEvenMergeSortS(int lo, int n)
    {
        if (n>1)
        {
            int m=n/2;
            oddEvenMergeSortS(lo, m);
            oddEvenMergeSortS(lo+m, m);
            oddEvenMerge(lo, n, 1);
        }
    }

    void oddEvenMergeSortP(int lo, int n)
    {
        if (n>1)
        {
            int m=n/2;

            		oddEvenMergeSortP(lo, m);
 
                    oddEvenMergeSortP(lo+m, m);
            oddEvenMerge(lo, n, 1);

        }
    }

	void sortS()
    {
        oddEvenMergeSortS(0, a.size());
    }

    void sortP(int t)
    {
		if(t==2){
			 #pragma omp parallel sections num_threads(2)
             {
             	#pragma omp section
            		oddEvenMergeSortP(0, a.size()/2);
            	 #pragma omp section
                    oddEvenMergeSortP(a.size()/2, a.size()/2);

             }
            oddEvenMerge(0, a.size(), 1);
		}
		else if(t==4){
			#pragma omp parallel sections num_threads(4)
             {
             	#pragma omp section
            		oddEvenMergeSortP(0, a.size()/4);
            	 #pragma omp section
                    oddEvenMergeSortP(a.size()/4, a.size()/4);
				#pragma omp section
            		oddEvenMergeSortP(a.size()/2, a.size()/4);
            	 #pragma omp section
                    oddEvenMergeSortP(a.size()/2 + a.size()/4, a.size()/4);
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
			oddEvenMergeSortP(0,a.size());
		}
			



    }

int main()
{
//	omp_set_num_threads(2);
	srand(time(NULL));

    cout<<"enter the number of elements to be sorted (number should be in the order of 2^n)";     //try to fix this issue 
    cin>>N;

	for (int i = 0; i < N; i++){
		a.push_back((rand() % 50));
		b.push_back(a[i]);

	}

	// printf("Original Array : ");
	// printArray(A);

    clock_t t;
    t=clock();
	sortP(1);
	t=clock()-t;
    printf("It parallel sort %d clicks %f seconds \n",t,((float)t)/CLOCKS_PER_SEC);

    clock_t t1;
    t1=clock();
    a=b;
	sortS();
	t1=clock()-t1;
    printf("It serial sort %d clicks %f seconds \n",t1,((float)t1)/CLOCKS_PER_SEC);

	// printf("Modified Array : ");
	// printArray(A);
	// printf("Modified Array : ");
	// printArray(B);

	return 0;
}
