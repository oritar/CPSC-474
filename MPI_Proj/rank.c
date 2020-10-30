#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])   {      	
    MPI_Init(&argc,&argv);
 //   printf("Hello, world!\n");
    MPI_Group group_world, odd_group, even_group;
    int i, p, Neven, Nodd, nonmembers[20], ierr, rank;

    MPI_Comm even_comm, odd_comm;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_group(MPI_COMM_WORLD, &group_world);

    Neven = (p+1)/2; /* processes of MPI_COMM_WORLD are divided */
    Nodd = p - Neven; /* into odd- and even-numbered groups */

    for (i=0; i<Neven; i++) { 
        /* "nonmembers" are even-numbered procs */
        nonmembers[i] = 2*i;
    }

    for(i = 0; i < Nodd; i++){
        nonmembers[i] = 2*i + 1;
    }
    
    // Odd group
    MPI_Group_excl(group_world, Nodd, nonmembers, &odd_group);

    // even group
    MPI_Group_excl(group_world, Neven, nonmembers, &even_group);

    MPI_Comm_create(MPI_COMM_WORLD, even_group, &even_comm);
    MPI_Comm_create(MPI_COMM_WORLD, odd_group, &odd_comm);

    i = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){
        i = 10;
    }

    MPI_Bcast(&i, 1, MPI_INT, 0, even_comm);
    printf("Process of rank %d has i = %d\n", rank, i);

    MPI_Finalize();

     return 0;
}

