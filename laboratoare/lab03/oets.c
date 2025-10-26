#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N; // dimensiunea vectorului
int P; // numarul de thread-uri
int *v; // vectorul de sortat
int *vQSort;
//odd-event transposition sort

pthread_barrier_t barrier;

void compare_vectors(int *a, int *b) {
	int i;

	for (i = 0; i < N; i++) {
		if (a[i] != b[i]) {
			printf("Sortare incorecta\n");
			return;
		}
	}

	printf("Sortare corecta\n");
}

void display_vector(int *v) {
	int i;
	int display_width = 2 + log10(N);

	for (i = 0; i < N; i++) {
		printf("%*i", display_width, v[i]);
	}

	printf("\n");
}

int cmp(const void *a, const void *b) {
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

void get_args(int argc, char **argv)
{
	if(argc < 3) {
		printf("Numar insuficient de parametri: ./oets N P\n");
		exit(1);
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);
}

void init()
{
	int i;
	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);

	if (v == NULL || vQSort == NULL) {
		printf("Eroare la malloc!");
		exit(1);
	}

	srand(42);

	for (i = 0; i < N; i++)
		v[i] = rand() % N;
}

void print()
{
	printf("v:\n");
	display_vector(v);
	printf("vQSort:\n");
	display_vector(vQSort);
	compare_vectors(v, vQSort);
}

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;

	// TODO: implementati aici OETS paralel
	int i, aux;
	int start, end;
	start = thread_id *(N/P);
	end = (thread_id + 1)*(N/P);
	if (thread_id == P - 1) {
		end = N;
	}
	start_even =(start %2 ==0)? start: start + 1; //par
	start_odd = (start %2 == 0)? start + 1: start; //impar
	for (int k = 0; k < N; k++) {
		// faza para
		for ( i = start_even; i < end-1 && i < N-1; i+=2)
		{
			if (v[i] > v[i+1]) {
				aux = v[i];
				v[i] = v[i+1];
				v[i+1] = aux;
			}
		}
		//bariera 
		pthread_barrier_wait(&barrier);
		// faza impara
		for ( i = start_odd; i < end-1 && i < N-1; i +=2)
		{
			if (v[i] > v[i+1]) {
				aux = v[i];
				v[i] = v[i+1];
				v[i+1] = aux;
			}
		}
		//bariera
		pthread_barrier_wait(&barrier);
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
	// se sorteaza vectorul etalon
	for (i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	// se creeaza thread-urile
	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	// se asteapta thread-urile
	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}
	// bubble sort clasic - trebuie transformat in OETS si paralelizat
	// se afiseaza vectorul etalon
	// se afiseaza vectorul curent
	// se compara cele doua
	print();

	free(v);
	free(vQSort);
	pthread_barrier_destroy(&barrier);

	return 0;
}
