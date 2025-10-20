#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define min(a, b) ((a) < (b) ? (a) : (b))

int *arr;
int array_size;
int num_threads;

// facem functia care va adauga 100 la fiecare element din array
void *add_100(void *arg)
{
  long id = *(long *)arg;

  // calculam sectiunea din vector pe care o va incrementa thread ul
  int start = id * (array_size / num_threads);
  int end = min((id + 1) * (array_size / num_threads), array_size);

  // incrementam fiecare element din segmentul alocat
  for (int i = start; i < end; i++)
  {
    for (int j = 0; j < 1000; j++)
    { // repetăm ca să dureze mai mult
      arr[i] += 100;
      arr[i] -= 100;
    }
    arr[i] += 100;
  }
  pthread_exit(NULL); // terminarea thread-ului
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  { // verificam daca avem 2 argumente din linia de comanda
    fprintf(stderr, "Specificati dimensiunea array-ului si numarul de thread-uri\n");
    exit(-1);
  }

  array_size = atoi(argv[1]); // converteste un sir de caractere la un intreg
  num_threads = atoi(argv[2]);

  arr = malloc(array_size * sizeof(int)); // aloca memorie pentru array
  for (int i = 0; i < array_size; i++)
  {
    arr[i] = i; // initializeaza array-ul cu valori de la 0 la array_size - 1
  }

  // afisam array ul initial
  for (int i = 0; i < array_size; i++)
  {
    printf("%d", arr[i]);
    if (i != array_size - 1)
    { // daca nu e ultimul element, punem spatiu intre ele
      printf(" ");
    }
    else
    {
      printf("\n");
    }
  }

  // TODO: aceasta operatie va fi paralelizata cu num_threads fire de executie
  // for (int i = 0; i < array_size; i++) {
  //   arr[i] += 100;
  // }
  // creem thread urile
  pthread_t threads[num_threads]; // vector de thread-uri
  int r;                          // pentru verificarea erorilor
  long ids[num_threads];          // vector de id-uri pentru thread-uri

  struct timespec start, finish;
  double elapsed;
  // start masurare timp real
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int i = 0; i < num_threads; i++)
  {
    ids[i] = i;
    r = pthread_create(&threads[i], NULL, add_100, &ids[i]); // crearea thread-ului
    if (r)
    {
      printf("Eroare la crearea thread-ului %d\n", i);
      exit(-1);
    }
  }
  // asteptam terminarea tuturor thread urilor
  for (int i = 0; i < num_threads; i++)
  {
    r = pthread_join(threads[i], NULL); // programul principal asteapta terminarea thread-ului
    if (r)
    {
      printf("Eroare la asteptarea thread-ului %d\n", i);
      exit(-1);
    }
  }

  // afisam array ul modificat
  printf("Array-ul dupa adaugarea lui 100 la fiecare element:\n");
  for (int i = 0; i < array_size; i++)
  {
    printf("%d", arr[i]);
    if (i != array_size - 1)
    {
      printf(" ");
    }
    else
    {
      printf("\n");
    }
  }
// stop masurare timp real
  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1e9; // secunde
  printf("Timpul de executie (varianta paralela) final: %f secunde\n", elapsed);
  free(arr);
  return 0;
}
//cd "/mnt/c/Users/Deni/Documents/FACULTATE/ANUL 3/APD - CTI/laborator 1/APD/laboratoare/lab01"
//gcc -o add_parallel add_parallel.c -lpthread
//./add_parallel 1000 4