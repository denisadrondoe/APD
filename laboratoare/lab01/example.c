#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#endif

// #define NUM_THREADS 4

void *f(void *arg)
{ // functia care va fi rulata de fiecare thread
  long id = *(long *)arg;
  printf("Hello World din thread-ul %ld!\n", id);
  pthread_exit(NULL); // terminarea thread-ului
}

int main(int argc, char *argv[])
{
  long cores;

  // Determină numărul de core-uri în funcție de sistemul de operare
#if defined(_WIN32) || defined(_WIN64)
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  cores = sysinfo.dwNumberOfProcessors;
#else
  cores = sysconf(_SC_NPROCESSORS_CONF);
#endif

  if (cores <= 0)
  {
    printf("Eroare la determinarea numarului de core-uri.\n");
    exit(-1);
  }

  pthread_t threads[cores]; // vector de thread-uri
  int r;                    // pentru verificarea erorilor
  long id;
  void *status;
  long ids[cores];

  for (id = 0; id < cores; id++)
  {
    ids[id] = id;
    r = pthread_create(&threads[id], NULL, f, &ids[id]); // crearea thread-ului

    if (r)
    {
      printf("Eroare la crearea thread-ului %ld\n", id);
      exit(-1);
    }
  }

  for (id = 0; id < cores; id++)
  {
    r = pthread_join(threads[id], &status); // programul principal asteapta terminarea thread-ului

    if (r)
    {
      printf("Eroare la asteptarea thread-ului %ld\n", id);
      exit(-1);
    }
  }

  return 0;
}
