#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#define NUM_THREADS 3

#if defined(_WIN32)|| defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#endif

void *f1(void *arg) {
    // functia care va fi rulata de fiecare thread
  long id = *(long *)arg;
  for(int i = 0; i < 50; i++)
  {
    printf("Iteratia %d: ", i);
    printf("Hello World din thread-ul %ld!\n", id);
  }
  pthread_exit(NULL);
}
void *f2(void *arg) {
    long id = *(long *)arg;
    for(int i=0; i < 50; i++)
    {
        printf("Iteratia %d: ", i);
        printf("Hello World din thread-ul %ld!\n", id);
    }
}


int main(int argc, char *argv[]) {

    long cores;
#if defined(_WIN32)|| defined(_WIN64)
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  cores = sysinfo.dwNumberOfProcessors;
#else
  cores = sysconf(_SC_NPROCESSORS_CONF); //din loborator 
#endif
    if (cores <= 0) {
        printf("Eroare la determinarea numarului de core-uri.\n");
        exit(-1);
    }   

  pthread_t threads[2];
  int r;
  long id;
  void *status;
  long ids[2] = {0, 1};

  r = pthread_create(&threads[0], NULL, f1, &ids[0]);

    if (r) {
      printf("Eroare la crearea thread-ului 0\n");
      exit(-1);
    }
    r = pthread_create(&threads[1], NULL, f2, &ids[1]);
    if(r) {
        printf("eroare la crearea threadului 1\n");
        exit(-1);
    }

  for (id = 0; id < 2; id++) {
    r = pthread_join(threads[id], &status);

    if (r) {
      printf("Eroare la asteptarea thread-ului %ld\n", id);
      exit(-1);
    }
  }

  return 0;
}