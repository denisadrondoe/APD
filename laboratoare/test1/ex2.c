#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N; //numarul de elemente din vector 
int P ; //numarul de thread uri 
int *v;
pthread_barrier_t barrier;

// TODO: paralelizati operatia din comentariul din functie
// in interiorul functiei respective
void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;

    if( thread_id == 0) {
        //minim
        int min = v[0];
        for(int i = 1; i< N; i++)
        {
            if(v[i] < min)
            {
                min = v[i];
            }
        }
        printf("Minimul este: %d\n", min);
        
    } 
    pthread_barrier_wait(&barrier); //punem o bariera astfel incat afisarea sa fie mereu in ordinea ceruta 
    //sa nu o ia inaimte alte thread uri 
    if(thread_id ==1) {
        //maxim 
        int max = v[0];
        for(int i = 1; i< N; i++)
        {
            if(v[i] > max)
            {
                max = v[i];
            }
        }
        printf("Maximul este: %d\n", max);
    } 
    pthread_barrier_wait(&barrier);
    if(thread_id == 2) {
        //suma
        int suma =0;
        for(int i = 0; i< N; i++)
        {
            suma += v[i];
        }
        printf("Suma este: %d\n", suma);

    } 
    pthread_barrier_wait(&barrier);
    if(thread_id == 3) {
        //produs
        int produs = 1;
        for(int i = 0; i< N; i++)
        {
            produs = (produs * v[i]) % 1337;
        }
        printf("produsul este: %d\n", produs);
    }

	pthread_exit(NULL);
}

void get_args(int argc, char **argv)
{
	if(argc < 2) {
		printf("Numar insuficient de parametri: ./program N\n");
		exit(1);
	}

	N = atoi(argv[1]);
}

void init()
{
    srand(123); //constanta
	v = malloc(sizeof(int) * N);

	if (v == NULL ) {
		printf("Eroare la malloc!");
		exit(1);
	}

	int i;

	for (i = 0; i < N; i++) {

		v[i] = rand() % 100;
    }
}

void print(int *v)
{
	int i;
	for (i = 0; i < N; i++) {
		printf("%i\t", v[i]);
	}
}

int main(int argc, char *argv[])
{
	int i;

	get_args(argc, argv);
	init();

	pthread_t tid[4];
	int thread_id[4];

    pthread_barrier_init(&barrier, NULL, 4);

	for (i = 0; i < 4; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	for (i = 0; i < 4; i++) {
		pthread_join(tid[i], NULL);
	}
    pthread_barrier_destroy(&barrier);
	return 0;
}