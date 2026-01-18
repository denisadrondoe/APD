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
    //Implementați operația de reduction, unde fiecare proces are o valoare proprie, 
    // prin care avem o colecție de valori (a nu se folosi MPI_Reduction)

    int value = rank;

    for (int i = 2; i <= procs; i *= 2) {
        // TODO
        if(rank % i ==0)  //primeste
        {
            //primeste de la procesul cu rank ul [rank + i/2] 
            int recv_value;
            MPI_Recv(&recv_value, 1, MPI_INT, rank + i/2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value += recv_value; //aduna valoarea primita de la proces 
            
        }
        else if ( rank % (i/2) == 0) { //trimite 
            //trimite la procesul cu rank ul [rank - i/2]
            MPI_Send(&value, 1, MPI_INT, rank - i/2, 0, MPI_COMM_WORLD);
            //break;
        }
    }

    if (rank == MASTER) {
        printf("Result = %d\n", value);
    }

    MPI_Finalize();

}

