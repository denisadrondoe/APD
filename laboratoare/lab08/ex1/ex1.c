#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    // Implementați algoritmul inel folosind MPI, unde procesul cu 
    // id-ul 0 trimite un număr aleatoriu către procesul 1 (vecinul său),
    //  iar apoi celelalte noduri vor primi numărul
    //  de la procesul precedent, îl incrementează cu 2 și îl trimit la 
    // următorul proces 
    // (de exemplu procesul 2 primește valoarea de la procesul 1, 
    // o incrementează și o trimite mai departe procesului 3),
    //  totul terminându-se când valoarea ajunge la procesul 0.
    //  Pentru fiecare proces trebuie să afișați rangul acestuia
    //  și valoarea primită

    int recv_num;

    // First process starts the circle.

    if (rank == 0) {
        // First process starts the circle.
        // Generate a random number.
        srand(time(NULL));
        int random_num = rand() % 100; //  numar aleatoriu intre 0 99
        printf("Procesul %d a generat și trimis: %d\n", rank, random_num);
        // Send the number to the next process.
        MPI_Send(&random_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

         // Primește numărul final înapoi de la ultimul proces.
        MPI_Recv(&recv_num, 1, MPI_INT, numtasks - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Procesul %d a primit valoarea finală: %d\n", rank, recv_num);
        fflush(stdout); //pentru a afisa in ordinea corecta

    } else if (rank == numtasks - 1) {
        // Last process close the circle.
        // Receives the number from the previous process.
        MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Procesul %d a primit: %d\n", rank, recv_num);
        // Increments the number.
        recv_num += 2;
        // Sends the number to the first process.
        MPI_Send(&recv_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    } else {
        // Middle process.
        // Receives the number from the previous process.
        MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Procesul %d a primit: %d\n", rank, recv_num);
        // Increments the number.
        recv_num += 2;
        // Sends the number to the next process.
        MPI_Send(&recv_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();
// mpicc -o ex1 ex1.c
// mpirun -np 4 ./ex1
}

