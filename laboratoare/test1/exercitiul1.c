#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N;
int P;
int **a;
int **b;
int **c;
pthread_mutex_t mutex;

// TODO: paralelizati operatia din comentariul din functie
// in interiorul functiei respective
void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;
    int start = thread_id * (double) N/P;
    int end = (thread_id + 1) *(double) N/P;
    if(end > N)
    {
        end = N;
    }
    int i, j, k;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
            int local_sum = 0;
			for (k = start; k < end; k++) {
				local_sum += a[i][k] * b[k][j];
			}
            pthread_mutex_lock(&mutex);

            c[i][j] += local_sum;

            pthread_mutex_unlock(&mutex);

		}
	}

	pthread_exit(NULL);
}

void get_args(int argc, char **argv)
{
	if(argc < 3) {
		printf("Numar insuficient de parametri: ./program N P\n");
		exit(1);
	}

	N = atoi(argv[1]);
    if(N > 1500)
    {
        printf("parametru N trebuie sa fie <=1500 \n");
		exit(1);
    }
	P = atoi(argv[2]);
}

void init()
{
	a = malloc(sizeof(int *) * N);
	b = malloc(sizeof(int *) * N);
	c = malloc(sizeof(int *) * N);

	if (a == NULL || b == NULL || c == NULL) {
		printf("Eroare la malloc!");
		exit(1);
	}

	int i, j;
	for (i = 0; i < N; i++) {
		a[i] = malloc(sizeof(int) * N);
		b[i] = malloc(sizeof(int) * N);
		c[i] = malloc(sizeof(int) * N);

		if(a[i] == NULL || b[i] == NULL || c[i] == NULL) {
			printf("Eroare la malloc!");
			exit(1);
		}
	}

	for (i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			c[i][j] = 0;
            a[i][j] = 7 + (i+j)%100;
            b[i][j] = 50 + (i+j)%100;
		}
	}
}

void print(int **mat)
{
	int i, j;

	for (i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			printf("%i\t", mat[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	int i;

	get_args(argc, argv);
	init();

	pthread_t tid[P];
	int thread_id[P];
    pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	print(c);
    pthread_mutex_destroy(&mutex);

	return 0;
}