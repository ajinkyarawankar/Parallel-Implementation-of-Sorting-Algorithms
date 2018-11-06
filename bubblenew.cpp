#include<iostream>
#include<vector>
#include<omp.h>
#include<thread>
#include<time.h>
using namespace std;




void bubbleSortSerial(vector<int> &arr){
	int size=arr.size();
	int temp;
	for (int i=1;i<size;i++){
		int flag=0;
		for(int j=0;j<size-i;j++){
			if(arr[j]>arr[j+1]){
				temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
				flag=1;
			}
		}
		if(flag==0)
			break;
	}
	
}


void bubbleSortParallel(vector<int> &arr){
	int size=arr.size();
	int temp;
	#pragma omp parallel for
	for (int i=0;i<size-1;i++){
			if(i%2==0){
				// omp_set_num_threads(16);
				#pragma omp parallel for
				for(int j=0;j<=size/2-1;j++){
						if(arr[2*j]>arr[2*j+1]){
							temp=arr[2*j];
							arr[2*j]=arr[2*j+1];
							arr[2*j+1]=temp;
						
						}
					}	
				}
			
			else{
				// omp_set_num_threads(16);
				#pragma omp parallel for
				for(int j=0;j<=size/2-2;j++){
						if(arr[2*j+1]>arr[2*j+2]){
							temp=arr[2*j+1];
							arr[2*j+1]=arr[2*j+2];
							arr[2*j+2]=temp;
						}
					}	
				}	
	}
		
}

int main(){
	int n;
	cin>>n;
// omp_set_num_threads(16);
	vector<int> arr(n);
	vector<int> brr(n);

	for(int i=0;i<n;i++){

		cin>>arr[i];
		brr[i]=arr[i];
	}
	

    clock_t t;
    t=clock();
    bubbleSortSerial(arr); 
    t=clock()-t;
    printf("It serial sort %d clicks %f seconds \n",t,((float)t)/CLOCKS_PER_SEC);

    clock_t t1;
    t1=clock();
    bubbleSortParallel(brr); 
    t1=clock()-t1;
  //   	for(int i=0;i<n;i++)
		// cout<<brr[i]<<endl;

    printf("It parallel sort %d clicks %f seconds \n",t1,((float)t1)/CLOCKS_PER_SEC);
	
	return 0;
}