#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    // Implementați un program în MPI în care 
    // procesul 0 trimite o valoare generată aleatoriu
    //  către celelalte procese, folosind MPI_Bcast.
    //   Atenție: Nu folosiți MPI_Recv.

    int rand_num;

    // Root process generates a random number.
    if(rank == ROOT) {
        srand(time(NULL));
        rand_num = rand() % 100; // random number between 0 and 99
        printf("Process [%d] generated %d.\n", rank, rand_num);
        fflush(stdout);
    }
    // Broadcasts to all processes.
    // MPI_Bcast este o operație COLECTIVĂ - trebuie apelată de TOATE procesele, nu doar de ROOT!
    MPI_Bcast(&rand_num, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    printf("Process [%d], after broadcast %d.\n", rank, rand_num);

    MPI_Finalize();

}

