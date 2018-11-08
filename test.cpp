

#include<iostream>                                                                              //for std::cout ,std::cin
#include<omp.h>           
#include<time.h>
#include<cstdlib>
using namespace std;

void ascendingSwap(int index1, int index2, int *ar)                                           //swap two values such that they appear in ascending order in the array
{
	if (ar[index2] < ar[index1])
	{
		int temp = ar[index2];
		ar[index2] = ar[index1];
		ar[index1] = temp;
	}
}
void decendingSwap(int index1, int index2, int *ar)                                           //swap two values such that they appear in decending order in the array
{
	if (ar[index1] < ar[index2])
	{
		int temp = ar[index2];
		ar[index2] = ar[index1];
		ar[index1] = temp;
	}
}
void bitonicSortFromBitonicSequence(int startIndex, int lastIndex, int dir, int *ar)     //form a increaseing or decreasing array when a bitonic input is given to the function
{
	if (dir == 1)
	{
		int counter = 0;                                                                    //counter to keep track of already swapped elements ,, parallelising this area results in poor performance due to overhead ,,need to fix
		int noOfElements = lastIndex - startIndex + 1;
		for (int j = noOfElements / 2; j > 0; j = j / 2)
		{
			counter = 0;
			for (int i = startIndex; i + j <= lastIndex; i++)
			{
				if (counter < j)
				{
					ascendingSwap(i, i + j, ar);
					counter++;

				}
				else
				{
					counter = 0;
					i = i + j - 1;

				}
			}
		}

	}
	else                                                                                    //decending sort
	{
		int counter = 0;
		int noOfElements = lastIndex - startIndex + 1;
		for (int j = noOfElements / 2; j > 0; j = j / 2)
		{
			counter = 0;
			for (int i = startIndex; i <= (lastIndex - j); i++)
			{
				if (counter < j)
				{
					decendingSwap(i, i + j, ar);
					counter++;

				}
				else
				{
					counter = 0;
					i = i + j - 1;

				}
			}
		}
	}

}
void bitonicSequenceGeneratorP(int startIndex, int lastIndex, int *ar)                         //generate a bitonic sequence  from a a random order
{
	int noOfElements = lastIndex - startIndex + 1;
	for (int j = 2; j <= noOfElements; j = j * 2)
	{
		#pragma omp parallel for                                                       //parallel implementation results in most performance gains here
		for (int i = 0; i < noOfElements; i = i + j)
		{
			if (((i / j) % 2) == 0)                                                               //extra computation results in drastically lower performance ,need to fix
			{
				bitonicSortFromBitonicSequence(i, i + j - 1, 1, ar);
			}
			else
			{
				bitonicSortFromBitonicSequence(i, i + j - 1, 0, ar);
			}
		}
	}
}

void bitonicSequenceGeneratorS(int startIndex, int lastIndex, int *ar)                         //generate a bitonic sequence  from a a random order
{
	int noOfElements = lastIndex - startIndex + 1;
	for (int j = 2; j <= noOfElements; j = j * 2)
	{
		for (int i = 0; i < noOfElements; i = i + j)
		{
			if (((i / j) % 2) == 0)                                                               //extra computation results in drastically lower performance ,need to fix
			{
				bitonicSortFromBitonicSequence(i, i + j - 1, 1, ar);
			}
			else
			{
				bitonicSortFromBitonicSequence(i, i + j - 1, 0, ar);
			}
		}
	}
}

int main()                                                                                        //main driver function
{
	std::cout << "hello" << endl;
	omp_set_dynamic(0);                                                                          //disabled so that the os doesnt override the thread settings                                                     
	//int maxNumberOfThreads = omp_get_num_procs();   
	//cout<<maxNumberOfThreads<<endl;                                             //gives number of logical cores
	omp_set_num_threads(8);                                                     //set the no of threads
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
	bitonicSequenceGeneratorP(0, N - 1, A);
	t = clock() - t;
	printf("It parallel sort %d clicks %f seconds \n", t, ((float)t) / CLOCKS_PER_SEC);

	clock_t t1;
	t1 = clock();
	bitonicSequenceGeneratorS(0, N - 1, B);
	t1 = clock() - t1;
	printf("It serial sort %d clicks %f seconds \n", t1, ((float)t1) / CLOCKS_PER_SEC);
	
//	for (int i = 0; i < N; i++) {
//	cout<<A[i]<<endl;
//
//	}
}
