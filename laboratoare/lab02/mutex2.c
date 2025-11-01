#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

int a = 0;
//pthread_mutex_t mutex;

//declaram mutex ul prin structura
typedef struct {
    pthread_mutex_t mutex;
} thread_data;

// TODO: adaugati mutexul in functia de mai jos
void *f(void *arg)
{
    thread_data *data = (thread_data *)arg;

    pthread_mutex_lock(&data->mutex);
	a += 2;
    pthread_mutex_unlock(&data->mutex);

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i, r;
	void *status;
	pthread_t threads[NUM_THREADS];
	//int arguments[NUM_THREADS];
    thread_data data;

    pthread_mutex_init(&data.mutex, NULL);

	for (i = 0; i < NUM_THREADS; i++) {
		//arguments[i] = i;
		r = pthread_create(&threads[i], NULL, f, &data);

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

	printf("a = %d\n", a);
    pthread_mutex_destroy(&data.mutex);

	return 0;
}