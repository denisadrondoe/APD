#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 1000
#define MASTER 0

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
	int displayWidth = 2 + log10(v[N-1]);
	for(i = 0; i < N; i++) {
		printf("%*i", displayWidth, v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}
 
int main(int argc, char * argv[]) {
	int rank, i, j;
	int nProcesses;
	MPI_Init(&argc, &argv);
	int pos[N];
	int sorted = 0;
	int *v = (int*)malloc(sizeof(int)*N);
	int *vQSort = (int*)malloc(sizeof(int)*N);

	for (i = 0; i < N; i++)
		pos[i] = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

    if (rank == MASTER) {
        // generate random vector
		srandom(42);
		for(i = 0; i < N; i++)
			v[i] = random() % 200;
    }

    // send the vector to all processes
	//trimite tot vectorul celorlalte procese 
    MPI_Bcast(v, N, MPI_INT, MASTER, MPI_COMM_WORLD);

	if(rank == 0) {
		// DO NOT MODIFY
		displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for(i = 0; i < N; i++)
			vQSort[i] = v[i];
		qsort(vQSort, N, sizeof(int), cmp);

		// calculeaza bucata de rank sort pentru bucata lui din vector 
		int start = 0;
		int end = N / nProcesses;

		// calculează pozițiile pentru bucata lui
		for (i = start; i < end; i++) {
			for (j = 0; j < N; j++) {
				if (v[j] < v[i] || (v[j] == v[i] && j < i)) {
					pos[i]++;
				}
			}
		}

        // primeste noile pozitii calculate de fiecare proces
		for (int p = 1; p < nProcesses; p++) {
		int start = p * (N / nProcesses);
		int end = (p + 1) * (N / nProcesses);

		MPI_Recv(&pos[start], end - start, MPI_INT,
				p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		//aranjeaza elementele conform pozitiilor primite
		int temp[N];
		for(i = 0; i < N; i++) {
			temp[pos[i]] = v[i];
		}

		for(i = 0; i < N; i++) {
			v[i] = temp[i];
		}
		// DO NOT MODIFY
		displayVector(v);
		compareVectors(v, vQSort);
	} else {
		
        // compute the positions
		int start = rank * (N / nProcesses);
		int end = (rank + 1) * (N / nProcesses);

		// fiecare proces este responsabil pentru calcularea unei anumite parti din vector 
		for (i = start; i < end; i++) {
			for (j = 0; j < N; j++) {
				if (v[j] < v[i] || (v[j] == v[i] && j < i)) {
					pos[i]++;
				}
			}
		}
        // toate procesele trimit procelului master noile pozitii calculate 
		MPI_Send(&pos[start], end - start, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
