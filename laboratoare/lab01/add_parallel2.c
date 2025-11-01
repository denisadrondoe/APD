#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define min(a, b) ((a) < (b) ? (a) : (b))
/*
    schelet pentru exercitiul 5
*/

int *arr;
int array_size;
int num_threads;

void *f(void *arg) {
    long id = *(long *)arg;
    int start = id * (double) array_size/num_threads;
    int end = min((id + 1) * (double) array_size/num_threads, array_size);

    for (int i = start; i < end; i++) {
        for(int j=0; j<1000; j++)
        {
            arr[i] += 100;
            arr[i] -= 100;
        }
    arr[i] += 100;
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Specificati dimensiunea array-ului si numarul de thread-uri\n");
    exit(-1);
  }

  array_size = atoi(argv[1]);
  num_threads = atoi(argv[2]);

  pthread_t threads[num_threads];
  int r;
  long id;
  void *status;
  long ids[num_threads];

  arr = malloc(array_size * sizeof(int));
  for (int i = 0; i < array_size; i++) {
    arr[i] = i;
  }

  //afisam arrayul initial
  for (int i = 0; i < array_size; i++) {
    printf("%d", arr[i]);
    if (i != array_size - 1) {
      printf(" ");
    } else {
      printf("\n");
    }
  }
  for ( int i=0; i < num_threads; i++) {
    ids[i] = i;
    r = pthread_create(&threads[i], NULL, f, &ids[i]);
    if (r) {
      printf("Eroare la crearea thread-ului %d\n", i);
      exit(-1);
    }
  }

  //asteptam incheierea fiecarui thread 
  for (int i = 0; i< num_threads; i++)
  {
    r = pthread_join(threads[i], & status);
  }

  // TODO: aceasta operatie va fi paralelizata cu num_threads fire de executie
// afisam arrayul final 
  for (int i = 0; i < array_size; i++) {
    printf("%d", arr[i]);
    if (i != array_size - 1) {
      printf(" ");
    } else {
      printf("\n");
    }
  }

  return 0;
}