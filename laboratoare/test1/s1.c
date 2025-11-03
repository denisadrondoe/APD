#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N;
int P;
int X;
int *v;
// int *Nr_aparitii; 
// int *Suma_pozitii;

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
	if(argc < 4) {
		printf("Numar insuficient de parametri: ./merge N P X (N trebuie sa fie putere a lui 2)\n");
		exit(1);
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);
    X = atoi(argv[3]);
    if (N <= 0 || N > 10000 || P <= 0 || P > 8 || X < 0 || X >= 5) {
        exit(1);
    }
}

void init()
{
	int i;
	v = malloc(sizeof(int) * N);

	if (v == NULL ) {
		printf("Eroare la malloc!");
		exit(1);
	}

	for (i = 0; i < N; i++)
		v[i] = (i + 3) % 5;
}

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;
    int nr=0, sum = 0;
    int start = thread_id*(double)N/P;
    int end = (thread_id+1)*(double)N/P;

    if(end == P-1) {
        end = N;
    }

    // int nr_aparitii_thread = 0;
    // int suma_pozitii_thread = 0;

	for( int i=start; i< end; i++)
    {
        if(v[i] == X)
        {
            //modificam v[i] = 1 daca X apare pe pozitia I, restul sunt 0
            v[i] = 1;
            // nr_aparitii_thread++;
            // suma_pozitii_thread += i;
        }
        else
        {
            v[i] = 0;
        }
    }

    Nr_aparitii[thread_id] = nr_aparitii_thread;
    Suma_pozitii[thread_id] = suma_pozitii_thread;
    //barrier pentru a astepta ca toate threadurile sa termine
    pthread_barrier_wait(&barrier);
    if (thread_id ==0)
    {
        for(int i = 0; i< N; i++)
        {
            // suma += Suma_pozitii[i];
            // aparitii += Nr_aparitii[i];
            if(v[i] == 1)
            {
                nr++;
                sum+=i;
                
            }
        }
        printf("%d %d", nr, sum);
    }
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	get_args(argc, argv);
	init();

	int i;
	int thread_id[P];
	pthread_t tid[P];
    pthread_barrier_init(&barrier, NULL, P);

	// se creeaza thread-urile
	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	// se asteapta thread-urile
	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}
	free(v);
    pthread_barrier_destroy(&barrier);

	return 0;
}