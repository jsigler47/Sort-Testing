#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <climits>
using namespace std;

#define MAX_NUM 1000

int list[100000000];
int N;
bool Correct = true;
bool wise = false;

void test(int Alg);
void insertionSort();
void quickSort(int left, int right);
	int getRandPivot(int left, int right);
	void swap(int *num1, int *num2); // quickSort helper functions
	int partition(int l, int r);
void mergeSort(int left, int right);
	void merge(int left, int mid, int right);// mergeSort helper function.
void printList();
void genRandList(int x); //List generator functions
void genSortedList(int x);
void genRevList(int x);
bool verifySort(); //Verifies a list is sorted.
//========================================================================================
int main(int argc, char* argv[]){
	int AlgToTest, numTests;
	//Args: Progname, Correct (1 or 0), AlgorithmToTest (1: insertion, 2: NaiiveQuick 3: WiseQuick 4: Merge), numTests
	if(argc < 4){
		cout << "Insufficient arguments supplied." << endl 
		<< "Args: Progname, Correct (1 or 0), AlgorithmToTest"
		<< "(1: insertion, 2: NaiiveQuick 3: WiseQuick 4: Merge), input N." 
		<< endl;
		return 0;
	}
	else{//Set test vars based on command line args.
		if(atoi(argv[1]) == 0)
			Correct = false;
		AlgToTest = atoi(argv[2]);
		N = atoi(argv[3]);
	}
	//Just use random list for correctness testing. This will be different in timed tests as the list is generated before each tests.
	genRandList(N);

	if(Correct){
		cout << "Testing for correctness.";
		//cout << "Random list: ";
		//printList();
		cout << " Sorting.";
		switch(AlgToTest){
			case 1: insertionSort(); 
					break;
			case 2: quickSort(0, N - 1); 
					break;
			case 3: wise = true;
					quickSort(0, N - 1); 
					break;
			case 4: mergeSort(0, N - 1); 
					break;
			default: cout << "Invalid AlgorithmToTest, select 1-4" << endl; 
					return 0;
			//cout << "Sorted list: ";
			//printList();
		}
			cout << " Sorted. Verifying." << endl;
			bool Sorted = verifySort();
			if(Sorted)
				cout << "Verified. List is sorted." << endl;
			else
				cout << "Not verified. List is not sorted." << endl;
	}
	else{ //Performance testing
		int testInterval = 1000; 
		clock_t start, stop;
		double minTime = 10.0 / (double) CLOCKS_PER_SEC;
		double total;
		int i;

		for(i = 1; i <= N; i++) //Tests list sizes of 0 to N.
		{
			if(i % testInterval == 0){
			//Uncomment the input type needed to test. Sorted, Reverse, or Random.
			//genSortedList(i);
			//genRevList(i);
			genRandList(i);

			total = 0.0;

			start = clock();
			test(AlgToTest); //Time the algorithm being tested.
			stop = clock();

			total = (stop - start) / (double) CLOCKS_PER_SEC;

			cout << i << " " << total << endl; //Format for gnuplot
			}
		}
	}
}
void test(int Alg){
	switch(Alg){
		case 1: insertionSort(); 
				break;
		case 2: quickSort(0, N - 1); 
				break;
		case 3: wise = true;
				quickSort(0, N - 1); 
				break;
		case 4: mergeSort(0, N - 1); 
				break;
		default: cout << "Invalid AlgorithmToTest, select 1-4" << endl; 
				return;
	return;
	}
}
//========================================================================================
void insertionSort(){
	int i, j, token;
	for(i = 1; i < N; i++){
		j = i - 1;
		token = list[i];

		while(j >= 0 && list[j] > token){
			list[j + 1] = list[j];
			j = j - 1;
		}
		list[j + 1] = token;
	}
}
//========================================================================================
void quickSort(int left, int right){
	int pivIndex;
	if(left < right)
	{
		if(wise) //Get a random pivot.
			pivIndex = getRandPivot(left, right);
		else//Naiive pivot, starts with the rightmost index.
			pivIndex = partition(left, right);
		quickSort(left, pivIndex - 1);
		quickSort(pivIndex + 1, right);
	}
	return;
}

int getRandPivot(int left, int right){
	int piv, n, tmp;
	srand(time(NULL));
	n = rand();
	piv = left + n % (right - left + 1);
	swap(list[right], list[piv]);

	return partition(left, right);
}

void swap(int *num1, int *num2){
	int tmp = *num1;
	*num1 = *num2;
	*num2 = tmp;
}

int partition(int l, int r){
	int pivot, index, i;
	index = l;
	pivot = r;

	for(i = l; i < r; i++)
	{
		if(list[i] < list[pivot]){
			swap(list[i], list[index]);
			index++;
		}
	}
	swap(list[pivot], list[index]);
	return index;
}
//========================================================================================
void mergeSort(int left, int right){
	if(left < right)
	{
		int mid = left+(right-left)/2;
		mergeSort(left, mid);
		mergeSort(mid+1, right);

		merge(left, mid, right);
	}
}

void merge(int left, int mid, int right){
	int i, j, k;
	int num1 = mid - left + 1;
	int num2 = right - mid;

	int tmpL[num1], tmpR[num2];
	//Copy to temp arrays.
	for(i = 0; i < num1; i++)
		tmpL[i] = list[left + i];
	for(j = 0; j < num2; j++)
		tmpR[j] = list[mid + 1 + j];

	i = j = 0;
	k = left;
	//Go through both left and right arrays and merge them based on value.
	while(i < num1 && j < num2){
		if(tmpL[i] <= tmpR[j]){
			list[k] = tmpL[i];
			i++;
		}
		else{
			list[k] = tmpR[j];
			j++;
	
		}
		k++;
	}
	//Copy any remaining elements of the left and right temp arrays.
	while(i < num1)
	{
		list[k] = tmpL[i];
		i++;
		k++;
	}
	while(j < num2){
		list[k] = tmpR[j];
		j++;
		k++;
	}
}
//========================================================================================
void printList(){
	for(int i = 1; i < N; i++){
		cout << list[i] << " ";
	}
	cout << endl;
}
//========================================================================================
void genRandList(int x){
	srand(time(NULL));
	for(int i = 0; i < x; i++) //Generate random list of ints to tests.
		list[i] = rand() % MAX_NUM;
}
//========================================================================================
void genSortedList(int x){
	for(int i = 0; i < x; i++) //Generate sorted list of ints to tests.
		list[i] = i;
}
//========================================================================================
void genRevList(int x){
	for(int i = x; i > 0; i--) //Generate reversed list of ints to tests.
		list[i] = i;
}
//========================================================================================
bool verifySort(){
	for(int i = 0; i < N; i++){ //Verify that the sorted list is indeed sorted.
		if(i != 0)
			if(list[i-1] > list[i])
				return false;
	}
	return true;
}
