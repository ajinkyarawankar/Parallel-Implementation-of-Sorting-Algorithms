#include<iostream>
#include<vector>
#include<omp.h>
#include<thread>
#include<time.h>
using namespace std;

class BitonicSorter
{
private:
    vector<int> a;
	static const bool ASCENDING=true, DESCENDING=false;

	void bitonicSort(int lo, int n, bool dir)
    {
        if (n>1)
        {
            int m=n/2;
            #pragma omp parallel 
            {
                #pragma omp sections
                {
                    #pragma omp section
                    {
                        bitonicSort(lo, m, ASCENDING);
                    }
                    #pragma omp section
                    {
                        bitonicSort(lo+m, m, DESCENDING);
                    }
                }
            }
            
            // bitonicSort(lo, m, true);
            // bitonicSort(lo+m, m, false);
            bitonicMerge(lo, n, dir);
        }

    }

    

	void bitonicMerge(int lo, int n, bool dir)
    {
        if (n>1)
        {
            int m=n/2;
            for (int i=lo; i<lo+m; i++){

                // compare(i, i+m, dir);
                if (dir==(a[i]>a[i+m]))
                {
                    int t=a[i];
                    a[i]=a[i+m];
                    a[i+m]=t;
                }
            }

            // #pragma omp parallel
            // {
            //     #pragma omp sections
            //     {
            //         #pragma omp section
            //         {
            //             bitonicMerge(lo, m, dir);
            //         }   
            //         #pragma omp section
            //         {
            //             bitonicMerge(lo+m, m, dir);
            //         }
            //     }
            // }
                         bitonicMerge(lo, m, dir);
                         bitonicMerge(lo+m, m, dir);
        }
    }

	void compare(int i, int j, bool dir)
    {
        if (dir==(a[i]>a[j]))
            {
            	int t=a[i];
		        a[i]=a[j];
		        a[j]=t;
            }
    }

    void bitonicSortS(int lo, int n, bool dir)
    {
        if (n>1)
        {
            int m=n/2;
            bitonicSortS(lo, m, ASCENDING);
            bitonicSortS(lo+m, m, DESCENDING);
            bitonicMergeS(lo, n, dir);
        }
    }

    void bitonicMergeS(int lo, int n, bool dir)
    {
        if (n>1)
        {
            int m=n/2;
            for (int i=lo; i<lo+m; i++){

                // compare(i, i+m, dir);
                if (dir==(a[i]>a[i+m]))
                {
                    int t=a[i];
                    a[i]=a[i+m];
                    a[i+m]=t;
                }
            }
            bitonicMergeS(lo, m, dir);
            bitonicMergeS(lo+m, m, dir);
        }
    }

public:
	void sort(vector<int> a)
    {
        this->a=a;

        clock_t t;
        t=clock();
        #pragma omp parallel sections
        {
            #pragma omp section
            // // thread th1(&BitonicSorter::bitonicSort,this,0, a.size()/2, true);
            bitonicSort(0, a.size()/2, ASCENDING);
            #pragma omp section
            bitonicSort(0+a.size()/2, a.size()/2, DESCENDING);
            // // thread th2(&BitonicSorter::bitonicSort,this,0+a.size()/2, a.size()/2, false);

            // th1.join();
            // th2.join();
        }
        
        // #pragma omp barrier
        bitonicMerge(0, a.size(), ASCENDING);

        t=clock()-t;
        printf("It parallel sort %d clicks %f seconds \n",t,((float)t)/CLOCKS_PER_SEC);

        this->a=a;
        clock_t t1;
        t1=clock();
        bitonicSortS(0, a.size(), ASCENDING);
        t1=clock()-t1;
        printf("It serial sort %d clicks %f seconds \n",t1,((float)t1)/CLOCKS_PER_SEC);


    }

    vector<int> getArray(){
    	return a;
    }
};




int main(){
	vector<int> b;
    int n;
    for(int i=0;i<2048;i++){
        cin>>n;
        b.push_back(n);
    }
	vector<int> result;

    // #pragma omp_set_dynamic(0)
    omp_set_num_threads(2);
    cout<<omp_get_num_procs()<<endl;
	BitonicSorter s;
	OddEvenMergeSorter o;
	s.sort(b);
	result=s.getArray();
	// for(int i=0;i<result.size();i++)
	// 	cout<<result[i]<<" ";
	// cout<<endl;
	// o.sort(b);
	// result=o.getArray();
	// for(int i=0;i<result.size();i++)
	// 	cout<<result[i]<<" ";
	// cout<<endl;
	return 0;
}

