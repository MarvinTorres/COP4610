#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

//Comment out below statement and recompile to disable synchronization
#define PTHREAD_SYNC 1

#define true 1
#define false 0

#ifdef PTHREAD_SYNC
   pthread_mutex_t mutex;
   pthread_barrier_t barrier;
#endif

int SharedVariable = 0;

void* SimpleThread(void* threadid) {
   int num, val;
   long which = (long)threadid;
   for (num = 0; num < 20; num++) {
      #ifdef PTHREAD_SYNC
         //Ensure that at most one thread is modifying SharedVariable
         //This prevents the boundary case of two threads reading SharedVariable simultaenously
         pthread_mutex_lock(&mutex);
      #endif
      #ifndef PTHREAD_SYNC
         //Stall threads to showcase the boundary case of two threads reading the same
         //value in SharedVariable
         if (random() > RAND_MAX / 2) {
            usleep(10);
         }
      #endif
      val = SharedVariable;
      printf("*** thread %d sees value %d\n", which, val);
      SharedVariable = val + 1;
      #ifdef PTHREAD_SYNC
         pthread_mutex_unlock(&mutex);
      #endif
   }
   #ifdef PTHREAD_SYNC
      //wait for the rest of the threads to reach this point
      pthread_barrier_wait(&barrier);
   #endif
   val = SharedVariable;
   printf("Thread %d sees final value %d\n", which, val);
}
   

int main(int argc, char* argv[]) {
   //Start command line argument validation
   if (argc != 2)
   {
      fprintf(stderr, "usage: sync numThreads\n");
      return -1;
   }
   int isValidArg = false;
   char* rawArg = argv[1];
   int numThreads = 0;

   if (sscanf(rawArg, "%d", &numThreads) == 1 && numThreads >= 0) {
      isValidArg = true;
   }

   if (!isValidArg) {
      fprintf(stderr, "numThreads must be a non-negative integer.\n");
      return -1;
   }

   //End command line argument validation
   
   #ifdef PTHREAD_SYNC
      if (pthread_barrier_init(&barrier, NULL, numThreads)) {
         fprintf(stderr, "ERROR: could not initialize barrier\n");
         return -1;
      }

      if (pthread_mutex_init(&mutex, NULL)) {
         fprintf(stderr, "ERROR: could not initialize mutex\n");
         return -1;
      }
   #endif

   long i;
   int rc;
   pthread_t threads[numThreads];

   for (i = 0; i < numThreads; i++) {
      rc = pthread_create(&threads[i], NULL, SimpleThread, (void*) i);
      if (rc) {
         printf("ERROR: return code from pthread_create() is %d\n", rc);
         return -1;
      }
   }

   //block main thread until other threads terminate
   for (i = 0; i < numThreads; i++) {
      if (pthread_join(threads[i], NULL)) {
          printf("ERROR: could not join thread\n");
          return -1;
      }

   }
   #ifdef PTHREAD_SYNC
      pthread_mutex_destroy(&mutex);
   #endif
   pthread_exit(NULL);
   return 0;
}
