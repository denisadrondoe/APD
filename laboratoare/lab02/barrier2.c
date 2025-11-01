#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

//pthread_barrier_t barrier;

//declarata cu structura 
typedef struct {
    int id;  //thread id
    pthread_barrier_t *barrier;
}thread_data;
// TODO: de folosit bariera in aceasta functie
void *f(void *arg)
{
    //int thread_id = *(int *)arg;

    //prima data extragem thread_id
    thread_data *data = (thread_data *)arg;
    int thread_id = data->id;

	if (thread_id == 1) {
		printf("1\n");
	}
    pthread_barrier_wait(data->barrier);

	if (thread_id == 0) {
		printf("2\n");
	}

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int i, r;
	void *status;
	pthread_t threads[NUM_THREADS];
	//int arguments[NUM_THREADS];
    thread_data data[NUM_THREADS];
    pthread_barrier_t barrier;

    pthread_barrier_init(&barrier, NULL, NUM_THREADS); 

	for (i = 0; i < NUM_THREADS; i++) {
		//arguments[i] = i;
		data[i].id = i;
		data[i].barrier = &barrier;
		r = pthread_create(&threads[i], NULL, f, &data[i]);

		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

	for (i = 0; i < NUM_THREADS; i++) {
		r = pthread_join(threads[i], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}
    pthread_barrier_destroy(&barrier);

	return 0;
}