#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])   {    
    int n = 5; // n =100	
    int a[5];   
    int i;
	for(i = 0; i < n; i++) a[i] = 0;   
    int rank;   
    int size;    	
    MPI_Init(&argc,&argv);     
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);	
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    printf("Process of rank %d has a[3]=%d \n", rank, a[3]);   
    if ( rank == 0 )    	
    	for ( i = 0; i < n; i++ )     a[i] = i;	
 	MPI_Bcast(&a, n, MPI_INT, 0, MPI_COMM_WORLD);  
     printf("After broadcast, process of rank %d has a[3]=%d \n", rank, a[3]);   
	MPI_Finalize();
     return 0;
}

