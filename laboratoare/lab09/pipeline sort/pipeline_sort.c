#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int N;

void compareVectors(int * a, int * b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}

// Use 'mpirun -np 20 --oversubscribe ./pipeline_sort' to run the application with more processes
int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

	if(rank==0) { // This code is run by a single process
		int intialValue = -1;
		int sorted = 0;
		int aux;
		int *v = (int*)malloc(sizeof(int) * (nProcesses - 1));
		int *vQSort = (int*)malloc(sizeof(int) * (nProcesses - 1));
		int i, val;

		// generate the vector v with random values
		// DO NOT MODIFY
		srandom(42);
		for(i = 0; i < nProcesses - 1; i++)
			v[i] = random() % 200;
		N = nProcesses - 1;
		displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for(i = 0; i < nProcesses - 1; i++)
			vQSort[i] = v[i];
		qsort(vQSort, nProcesses - 1, sizeof(int), cmp);

		//initializeaza valoarea procesului i cu -1 
		for(i = 1; i < nProcesses ; i ++) {
			MPI_Send(&intialValue, 1, MPI_INT, i , 0, MPI_COMM_WORLD);
		}
		//INTRAM IN PIPELINE 
		// TODO send the vector to rank == 1
		//trimitem toate valorile vectorului procesului 1
		for(i = 1; i < nProcesses; i ++) {
			MPI_Send(&v[i - 1], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		}
		//primeste valorile sortate inapoi de la toate procesele 
		for(i = 1; i < nProcesses; i ++) {
			MPI_Recv(&v[i - 1], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		displayVector(v);
		compareVectors(v, vQSort);
	} else {
		// TODO sort the vector v using N processes (N == nProcesses - 1)
		int initial, new, i;
		//primeste aloare -1 cu care este initializat de la procesul 0 
		MPI_Recv(&initial, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
		printf("Process %d received initial value %d\n", rank, initial);

		for(i = 0; i < nProcesses - rank; i ++) {  // numarul de elemente pe care le primeste fiecare worker din stanga (numarul de elemente nesortate adica)
			MPI_Recv(&new, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Process %d received value %d\n", rank, new);

			// daca procesul nu are inca o valoare salvata o ia pe prima primita 
			if(initial == -1) {
				initial = new;
			} else {
				if(new >= initial) { // daca valoarea primita este mai >= decat cea salvata
					//trimite valoarea mai mare la procesul urmator
					MPI_Send(&new, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
				} else { //daca valoarea primita este mai mica decat cea salvata
					//trimitem mai departe valoarea salvata 
					MPI_Send(&initial, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
					initial = new; // initial devine noua valoare mai mica 
				}
			}
		}
		
		//trimite valoarea salvata inapoi la procesul 0
		MPI_Send(&initial, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); //trimite catre proceul 0 vectorul sortat 
	}

	MPI_Finalize();
	return 0;
}
