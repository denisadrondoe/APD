#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main (int argc, char *argv[])
{
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Implementați operația de broadcast, prin care procesul 0 trimite
    //o valoare tuturor proceselor (a nu se folosi MPI_Bcast)
    int value;

    if (rank == MASTER) {
        value = 7;
    }

    for (int i = 1; i < procs; i *= 2) {
        // TODO
        if(rank < i && rank + i < procs) {
            MPI_Send(&value, 1, MPI_INT, rank + i, i, MPI_COMM_WORLD);
        }
        else if(rank >= i && rank < 2 * i) {
            MPI_Recv(&value, 1, MPI_INT, rank - i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    printf("Process [%d] has value = %d\n", rank, value);

    MPI_Finalize();

}

