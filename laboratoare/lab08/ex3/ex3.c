#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MULTI 5 // chunk dimension
#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); //cate procese ruleaza
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); //id ul procesului curent
    MPI_Get_processor_name(hostname, &len);

    int num_elements = MULTI * numtasks; // total elements 5*4=20 
    int *v_send = NULL; // full vector  vectorul complet , doar root il foloseste (20 de elemente   )
    int *v_recv = (int *)malloc(MULTI * sizeof(int)); // fiecare proces are 5 elemente 

    // ROOT process generates the values for the full vector.
    if (rank == ROOT) {
        v_send = (int *)malloc(num_elements * sizeof(int));
        for (int i = 0; i < num_elements; i++) {
            v_send[i] = i;
        }

        printf("Process [%d]: Generated array: ", rank); 
        for(int i = 0; i < num_elements; i++) {
            printf("%d ", v_send[i]);
        }
        printf("\n");
        fflush(stdout);
    }

    // Scatter the vector to all processes.
    MPI_Scatter(v_send, MULTI, MPI_INT, //trimite din vsend cate MULTI elemente
         v_recv, MULTI, MPI_INT, //primeste in vrecv cate MULTI elemente
                ROOT, MPI_COMM_WORLD); //ROOT procesul care imparte datele

    /*
     * Prints the values received after scatter.
     * NOTE: If MULTI changed, also change this line.
     */
    printf("Process [%d]: have elements %d %d %d %d %d.\n", rank, v_recv[0],
            v_recv[1], v_recv[2], v_recv[3], v_recv[4]);
    fflush(stdout);

    // Each process increments the values of the partial vector received.
    for (int i = 0; i < MULTI; i++) {
        v_recv[i] += rank;
    }
    // Gathers the values from all the processes.
    MPI_Gather(v_recv, MULTI,  MPI_INT, //fiecare proces trimite din vrecv cate MULTI elemente
         v_send, MULTI, MPI_INT, //ROOT primeste toate elementele in vsend
          ROOT, MPI_COMM_WORLD); //ROOT colecteaza datele
    // The ROOT process prints the elements received.
    if (rank == ROOT) {
        printf("Process [%d]: Array after gather: ", rank); //ROOT afiseaza vectorul complet
        for(int i = 0; i < num_elements; i++) {
            printf("%d ", v_send[i]);
        }
        printf("\n");
        
        // Elibereaza memoria
        free(v_send);
    }

    free(v_recv);

    MPI_Finalize();

}

