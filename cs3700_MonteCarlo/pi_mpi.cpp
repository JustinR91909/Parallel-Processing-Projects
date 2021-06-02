#include "mpi.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <math.h>
#include <ctime>

#define ARRAY_SIZE 100

int main (int argc,  char *argv[]) {

   int myid, numprocs;
   int namelen;
   int* points = new int[ARRAY_SIZE];
   char processor_name[MPI_MAX_PROCESSOR_NAME];

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Get_processor_name(processor_name, &namelen);
 
   printf("Process %d on %s\n", myid, processor_name);

   int s = (int)floor(ARRAY_SIZE/numprocs);
   int s0 = s + ARRAY_SIZE%numprocs;

   int startIndex = s0 + (myid-1)*s;
   int endIndex = startIndex + s;

   double startwtime;
   if (myid == 0) {
      startwtime = MPI_Wtime();
   }

   int i;
   double x;
   double y;
   double distance;
   int inside;
   int outside;
   
   if (myid == 0) {
      // master worker - comput the master's numbers
      srand((unsigned)(myid));
      for (i=0; i<s0; i++) {
      	 x = ((double)rand()) / ((double)RAND_MAX);
      	 y = ((double)rand()) / ((double)RAND_MAX);
      	 distance = x*x + y*y;
      	 
      	 if(distance <= 1)
      	 	inside++;	
	 outside++;
      }
      printf("Process %d - startIndex 0 endIndex %d; inside %ld; outside %ld\n",
             myid, s0-1, inside, outside);
   } else {
      //slave's work
      srand((unsigned)(myid));
      for (i= startIndex; i<endIndex; i++) {
         x = ((double)rand()) / ((double)RAND_MAX);
      	 y = ((double)rand()) / ((double)RAND_MAX);
      	 distance = x*x + y*y;
      	 
      	 if(distance <= 1)
      	 	inside++;	
	 outside++;
      }
      printf ("Process %d - startIndex %d endIndex %d; inside %ld; outside %ld\n",
              myid, startIndex, endIndex-1, inside, outside);
   }

   int in = 0;
   int out = 0;
   MPI_Reduce(&inside, &in, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   MPI_Reduce(&outside, &out, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

   if (myid == 0) {
      double runTime = MPI_Wtime() - startwtime;
      printf("Execution time (sec) = %f\n",
             runTime);
      double ratio = ((double)in)/((double)out);
      double pi = 4*ratio;
      double delta = M_PI - pi;
      printf("Estimated pi = %f Delta = %f\n", 
	  		 pi, delta);
   }

   delete[] points;

   MPI_Finalize();
}
