#include <pthread.h>
#include <stdio.h>

#define MAX_FIB_COUNT 90

long long fibs[MAX_FIB_COUNT]; /* this data is shared by the thread(s) */
void *runner(void *param); /* threads call this function */

int main(int argc, char *argv[]) {
   pthread_t tid; /* the thread identifier */
   pthread_attr_t attr; /* set of thread attributes */
   
   if (argc != 2) {
      fprintf(stderr, "usage: a.out <integer value>\n");
      return -1;
   }

   if (atoi(argv[1]) < 0) {
      fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
      return -1;
   }

   if (atoi(argv[1]) > MAX_FIB_COUNT) {
      fprintf(stderr, "%d must be <= %d\n", atoi(argv[1]), MAX_FIB_COUNT);
      return -1;
   }

   /* get the default attributes */
   pthread_attr_init(&attr);
   /* create the thread */
   pthread_create(&tid, &attr, runner, argv[1]);
   /* wait for the thread to exit */
   pthread_join(tid, NULL);
   
   int numFibsGenerated = atoi(argv[1]);
   int i;
   for (i = 0; i < numFibsGenerated; i++)
   {
      printf("fib #%-2d: %20lld\n", i + 1, fibs[i]);
   }
   pthread_exit(0);
}

/* The thread will begin control in this function */
void *runner(void *param)
{
   int i, numFibsGenerated = atoi(param);
   fibs[0] = 0;
   fibs[1] = 1;

   for (i = 2; i < numFibsGenerated; i++) {
      fibs[i] = fibs[i - 1] + fibs[i - 2];      
   }

   pthread_exit(0);
}
