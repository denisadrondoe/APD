#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N;
int P;
int *v;  //rezultatul fiecarui thread
// int *Nr_aparitii; 
// int *Suma_pozitii;

pthread_barrier_t barrier;

// array-uri pentru rezultate
int result_int[4];      // sum, prod, min, max
double result_double;   // medie

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
		printf("Numar insuficient de parametri: ./merge N P X (N trebuie sa fie putere a lui 2)\n");
		exit(1);
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);
     (N <= 0 || N > 100 || P <= 0 || P > 5) {
        exit(1);
    }
    //alocam v inainte de a i pune valori 
    v = malloc(sizeof(int) * N);
    if(v == NULL) exit(1);

    for(int i=0 ; i< N; i++)
    {
        v[i] = atoi(argv[i+3]);
    }
}

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;

    switch(thread_id) {
        case 0: // sum
            result_int[0] = 0;
            for(int i = 0; i < N; i++) result_int[0] += v[i];
            break;
        case 1: // prod
            result_int[1] = 1;
            for(int i = 0; i < N; i++) result_int[1] *= v[i];
            break;
        case 2: // min
            result_int[2] = v[0];
            for(int i = 1; i < N; i++) if(v[i] < result_int[2]) result_int[2] = v[i];
            break;
        case 3: // max
            result_int[3] = v[0];
            for(int i = 1; i < N; i++) if(v[i] > result_int[3]) result_int[3] = v[i];
            break;
        case 4: // medie
            {
                int sum = 0;
                for(int i = 0; i < N; i++) sum += v[i];
                result_double = (double)sum / N;
            }
            break;
    }
    //punem o bariera sa stepte toate thread urile sa termine 
    pthread_barrier_wait(&barrier);
    if(thread_id == 0)
    {
        //afiseaza
        if(P == 1)
        {
            printf("sum: %d \n", result_int[0]);
        }
        else
        if(P == 2)
        {
            printf("sum: %d\n prod: %d\n", result_int[0], result_int[1]);
        }
        else
        if(P == 3)
        {
            printf("sum: %d\n prod: %d\n min: %d\n", result_int[0], result_int[1], result_int[2]);
        }
        else
        if(P == 4)
        {
            printf("sum: %d\n prod: %d\n min: %d\n max: %d\n", result_int[0], result_int[1], result_int[2], result_int[3]);
        }
        else
        if(P == 5)
        {
            printf("sum: %d\n prod: %d\n min: %d\n max: %d\n medie: %.0f\n", result_int[0], result_int[1], result_int[2], result_int[3], result_double);
        }
    }
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	get_args(argc, argv);

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