#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MASTER 0

int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

	if (rank == MASTER) { //preocesul MASTER citeste coeficientii din fisier 
		int polynomialSize, n;
		int x = 5; // valoarea cu care se calculeaza polinomul - f(5)

		/*
			in fisierul de intrare formatul este urmatorul:
			numarul_de_coeficienti
			coeficient x^0
			coeficient x^1
			etc.
		*/

		FILE * polFunctionFile = fopen(argv[1], "rt");
		fscanf(polFunctionFile, "%d", &polynomialSize);
		/*
			in array-ul a se vor salva coeficientii ecuatiei / polinomului
			de exemplu: a = {1, 4, 4} => 1 * (x ^ 2) + 4 * (x ^ 1) + 4 * (x ^ 0)
		*/
		float *a = malloc(sizeof(float)*polynomialSize);
		for (int i = 0; i < polynomialSize; i++) {
			fscanf(polFunctionFile, "%f", &a[i]);
			printf("Read value %f\n", a[i]);
			/*
				Se trimit coeficientii pentru x^1, x^2 etc. proceselor 1, 2 etc.
				Procesul 0 se ocupa de x^0 si are valoarea coeficientului lui x^0
			*/
			if (i > 0) { 
				printf("Sending %f to process %d\n", a[i], i);
				MPI_Send(&a[i], 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD); //trimite coeficientul a[i] procesului i 
			}
		}

		fclose(polFunctionFile);

		//PORNESTE PIPELINE UL 
		// Se trimite valoarea  x = 5 si suma partiala (in acest caz valoarea coeficientului lui x^0)
		MPI_Send(&x, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); //porneste pipeline trimitand x catre procesul 1
		MPI_Send(&a[0], 1, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD); //trimite suma partiala SUM = a[0] catre procesul 1
	} else {
		float val, sum;
		int x;
		/*
			se primesc: 
			- coeficientul corespunzator procesului (exemplu procesul 1 primeste coeficientul lui x^1)
			- suma partiala
			- valoarea x din f(x)
			si se calculeaza valoarea corespunzatoare pentru c * x^r, r fiind rangul procesului curent
			si c fiind coeficientul lui x^r, si se aduna la suma
		*/

		MPI_Recv(&val, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, NULL); //primeste valoarea coeficientului x^i; val = a[i]
		printf("Process %d received value %f\n", rank, val);

		MPI_Recv(&x, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, NULL); //primeste valoarea x = 5
		MPI_Recv(&sum, 1, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD, NULL); //primeste suma partiala calculata pana acum
		printf("Received x = %d and sum = %f\n", x, sum);
		sum += val * pow(x, rank); //calculeaza termenul propriu si il aduna la suma

		if (rank == nProcesses - 1) {
			printf("Polynom value is %f\n", sum);
		} else {
			// se trimit mai departe x si suma partiala catre urmatorul proces
			MPI_Send(&x, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			MPI_Send(&sum, 1, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
}
