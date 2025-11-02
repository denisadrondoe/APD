#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N;
int P;
int *v;
int *rank;
int *result;
pthread_barrier_t barrier;

void display_vector(int *v) {
	int i;
	int display_width = 2 + log10(N);

	for (i = 0; i < N; i++) {
		printf("%*i", display_width, v[i]);
	}

	printf("\n");
}

void get_args(int argc, char **argv)
{
	if(argc < 3) {
		printf("Numar insuficient de parametri: ./oets N P\n");
		exit(1);
	}

	N = atoi(argv[1]);
    if(N > 10000) {
        printf("N nu poate fi > 10000");
        exit(1);
    }
	P = atoi(argv[2]);
    if(P > 8) {
        printf("P nu poate fi > 8");
        exit(1);
    }
}

void init()
{
	int i;
    result = malloc(sizeof(int) *N);
    rank = malloc(sizeof(int) *N);

	
    if (result == NULL ) {
		printf("Eroare la malloc!");
		exit(1);
	}
    if (rank == NULL ) {
		printf("Eroare la malloc!");
		exit(1);
	}

    for (int i = 0; i < N; i++)
    {
        rank[i] = 0;  // inițializare
        result[i] = 0;
    }
}

void print()
{
	printf("vecorul initial v:\n");
	display_vector(v);
}

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;
    int start = thread_id * N/P;
    int end = (thread_id + 1) * N/P; 
    if(end > N)
    {
        end = N;
    }
    int i, j;
	// TODO: implementati aici Rank Sort paralel
    for( i = start; i< end; i++)
    {
        for( j = 0; j< N; j++)
        {
            if (v[i] > v[j] || (v[j] == v[i] && j < i))
                rank[i]++; // numara fata de cate numere e mai mare 
        }
    }
    //sincronizam astfel incat toate thread urile sa termine rank ul 
    pthread_barrier_wait(&barrier); 

    if(thread_id == 0) {
        for( i = 0; i< N; i++)
        {
            result[rank[i]] = v[i];
        }   
        display_vector(result);
    }
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	get_args(argc, argv);
	init();

	int i, aux;
	pthread_t tid[P];
	int thread_id[P];

    pthread_barrier_init(&barrier, NULL, P);
    v = malloc(sizeof(int) * N);
    if (v == NULL ) {
		printf("Eroare la malloc!");
		exit(1);
	}
    for(i = 0; i < N; i++) {
        v[i] = N - i + 5;
    }

	// se creeaza thread-urile
	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	// se asteapta thread-urile
	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

    print();
	//display_vector(result);
	free(v);
    pthread_barrier_destroy(&barrier);

	return 0;
}