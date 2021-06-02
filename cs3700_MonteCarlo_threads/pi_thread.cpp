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
#include <math.h>
#include <cmath>
#include <cstdlib>




using namespace std;

//constexpr long long size= 1000000;   
mutex myMutex;

void findInOut(unsigned long long& inside, unsigned long long& outside, unsigned long long beg, unsigned long long end){
	srand((unsigned)(time(0)));
	int in = 0;
	int out = 0;
	double x, y;
	double distance;
    for (auto it=beg; it<end; ++it) {
      	 x = ((double)rand()) / ((double)RAND_MAX);
      	 y = ((double)rand()) / ((double)RAND_MAX);
      	 distance = x*x + y*y;
      	 if(distance <= 1)
      	 	in++;	
		 out++;
    }
    lock_guard<mutex> myLock(myMutex);
    inside += in;
    outside += out;
}

int main(int argc,  char *argv[]){

  cout << endl;
  
  srand(time(0));
  
  if(argc != 3 || atoi(argv[1]) < 1 || atoi(argv[1]) > 10 || atoi(argv[2]) < 10 || atoi(argv[2]) > 1000000)
  {
  	  cout << "usage: monte [1...10] [10...1000000]" << endl;
  	  cout << endl;
  }
  else
  {
	  int threads = atoi(argv[1]);
	  long long size= atoi(argv[2]); 
	  
	  vector<int> points;
	  points.reserve(size);
	 
	  unsigned long long inside = 0;
	  unsigned long long outside = 0;
	  auto start = chrono::system_clock::now();

	  thread t[threads];
	  long long slice = (int)floor(size/threads);
	  cout << slice << endl;
	  long long sliceRemainder = size%threads;
	  cout << sliceRemainder << endl;
	  int startIdx = sliceRemainder + slice;
	  cout << startIdx << endl;
	  int endIdx = startIdx + slice;
	  
	  //Have first thread cover potential remainder
	  cout << "Thread[0] - slice [0:" << slice+sliceRemainder-1 << "]" << endl;
	  t[0] = thread(findInOut, ref(inside), ref(outside), 0, slice+sliceRemainder);
	  
	  for (int i = 1; i < threads; ++i) {
	    cout << "Thread[" << i << "] - slice ["
	         << startIdx << ":" << startIdx+slice-1 << "]" << endl;
	    t[i] = thread(findInOut, ref(inside), ref(outside), startIdx, startIdx+slice);
	    startIdx += slice;
	  }
	
	  for (int i = 0; i < threads; ++i)
	     t[i].join();
	
	  chrono::duration<double> dur= chrono::system_clock::now() - start;
	  cout << "Time: " << dur.count() << " seconds" << endl;
	  cout << "#inside: " << inside  << " #outside: " << outside << endl;
	  double ratio = ((double)inside)/((double)outside);
      double pi = 4*ratio;
      double delta = M_PI - pi;
      printf("Estimated pi = %f Delta = %f Ratio = %f\n", 
	  		 pi, delta, ratio);
	
	  cout << endl;
	}
}
