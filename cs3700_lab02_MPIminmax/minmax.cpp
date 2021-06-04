#include "mpi.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <math.h>

#define ARRAY_SIZE 1000000

int main (int argc,  char *argv[]) {

   int myid, numprocs;
   int namelen;
   int* numbers = new int[ARRAY_SIZE];
   char processor_name[MPI_MAX_PROCESSOR_NAME];
   srand((unsigned)(myid));
   

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Get_processor_name(processor_name, &namelen);
 
   printf("Process %d on %s\n", myid, processor_name);
 
   for (int i=0; i<ARRAY_SIZE; i++)
      numbers[i] = rand();  //could be randomly generated

   int s = (int)floor(ARRAY_SIZE/numprocs);
   int s0 = s + ARRAY_SIZE%numprocs;

   int startIndex = s0 + (myid-1)*s;
   int endIndex = startIndex + s;

   double startwtime;
   if (myid == 0) {
      startwtime = MPI_Wtime();
   }

   int i;
   int part_sum = 0;
   int part_max = numbers[0];
   int part_min = numbers[0];
   
   if (myid == 0) {
      // master worker - comput the master's numbers
      ;
      for (i=1; i<s0; i++) {
		  if(numbers[i] > part_max){
			 part_max = numbers[i];
		  }
        	  
          if(numbers[i] < part_min){
          	 part_min = numbers[i];
		  }
        	  
      }
      printf("Process %d - startIndex 0 endIndex %d; max %ld; min %ld\n",
             myid, s0-1, part_max, part_min);
   } else {
      //slave's work
      for (i= startIndex; i<endIndex; i++) {
         if(numbers[i] > part_max){
			 part_max = numbers[i];
		  }
          if(numbers[i] < part_min){
          	 part_min = numbers[i];
		  }
      }
      printf ("Process %d - startIndex %d endIndex %d; max %ld; min %ld\n",
              myid, startIndex, endIndex-1, part_max, part_min);
   }

   int sum = 0;
   int max = 0;
   int min = 0;
   MPI_Reduce(&part_max, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
   MPI_Reduce(&part_min, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

   if (myid == 0) {
      double runTime = MPI_Wtime() - startwtime;
      printf("Execution time (sec) = %f max = %ld min = %ld \n",
             runTime, max, min);
   }

   delete[] numbers;

   MPI_Finalize();
}



//have each slave get a portion of the number's array and dinf the min and max for those chunks
//return min and max for each chunk then do 1 final sort to find actual min and max
