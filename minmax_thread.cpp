#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

using namespace std;

constexpr long long size= 1000000;   
mutex myMutex;

void findMinMax(unsigned long long& min, unsigned long long& max, const vector<int>& val, 
   unsigned long long beg, unsigned long long end){
   long long localMin = 2147483647;
   long long localMax = 0;
    for (auto it= beg; it < end; ++it){
        if(val[it] < localMin){
        	localMin = val[it];
		}
		if(val[it] > localMax){
        	localMax = val[it];
		}
    }
    lock_guard<mutex> myLock(myMutex);
    if(localMin < min){
        min = localMin;
	}
	if(localMax > max){
        max = localMax;
	}
}

int main(){

  cout << endl;
  
  srand(time(0));

  vector<int> randValues;
  randValues.reserve(size);

  mt19937 engine (0);
  uniform_int_distribution<> uniformDist(1,1000000);
  for ( long long i=0 ; i< size ; ++i)
     randValues.push_back(rand());
 
  unsigned long long min = 2147483647;
  unsigned long long max = 0;
  auto start = chrono::system_clock::now();

  int threads = 8;
  thread t[threads];
  long long slice = size / threads;
  int startIdx=0;
  for (int i = 0; i < threads; ++i) {
    cout << "Thread[" << i << "] - slice ["
         << startIdx << ":" << startIdx+slice-1 << "]" << endl;
    t[i] = thread(findMinMax, ref(min), ref(max), ref(randValues), startIdx, startIdx+slice-1);
    startIdx += slice;
  }

  for (int i = 0; i < threads; ++i)
     t[i].join();

  chrono::duration<double> dur= chrono::system_clock::now() - start;
  cout << "Time for addition " << dur.count() << " seconds" << endl;
  cout << "Min: " << min  << " Max: " << max << endl;

  cout << endl;

}
