#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <list>
#include "mpi.h"

using namespace std;

struct loc{
    int value_;
    int row_;
    int col_;
};

string printList(list<loc>);

int main(int argc, char* argv[]){
    int rank, size, mtxSize, n, m;
    int** matrix;
    list<loc> finalList;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype dt_loc;
    MPI_Type_contiguous(3, MPI_INT, &dt_loc);
    MPI_Type_commit(&dt_loc);
    
    loc elem;
    int counter; // keep track of how many non-zero elements found
    int start, stop;
    
    if(rank == 0){
        ifstream file;
        file.open("mtx.txt");

        file >> n;
        file >> m;
        
        matrix = new int*[n];
        mtxSize = n*m;

        matrix[0] = new int[mtxSize];
        for(int i = 1; i < n; i++){
            matrix[i] = &matrix[0][i*m];
        }

        for (int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                file >> matrix[i][j];
            }
        } 

        MPI_Bcast(&mtxSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&(matrix[0][0]), n*m, MPI_INT, 0, MPI_COMM_WORLD);
    
        file.close();
    }
    else {
        MPI_Bcast(&mtxSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

        matrix = new int*[n];

        matrix[0] = new int[mtxSize];
        for(int i = 1; i < n; i++){
            matrix[i] = &matrix[0][i*m];
        }

        MPI_Bcast(&(matrix[0][0]), n*m, MPI_INT, 0, MPI_COMM_WORLD);

        if(rank == 1){
            start = 0;
            stop = (rank*(n/size) + (n/size));
        }
        else {
            start = (rank*(n/size));
            stop = (rank*(n/size) + (n/size));
        }

        for(int i = start; i < stop; i++){            
            for(int j = 0; j < m; j++){
                if(matrix[i][j] != 0){
                    elem = {matrix[i][j], i, j};
                    MPI_Send(&elem, 1, dt_loc, 0, 0, MPI_COMM_WORLD);
                    counter++;
                }
            }
        }
        MPI_Send(&counter, sizeof(counter), MPI_INT, 0, rank, MPI_COMM_WORLD);    
      }

    if(rank == 0){
        for(int i = 1; i < size; i++){
            MPI_Recv(&counter, sizeof(counter), MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for(int j = 0; j < counter; j++){
                MPI_Recv(&elem, 1, dt_loc, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                finalList.push_back(elem);
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0){
        cout << printList(finalList) << endl;
    }

    MPI_Finalize();
    return 0;
}

string printList(list<loc> pr){
    string print = "<--- BEGIN LIST ---> \n";
    loc elem;

    for(std::list<loc>::iterator it = pr.begin(); it != pr.end(); it++){
       print += "mtx[" + to_string(it->row_) + "][" + to_string(it->col_) + "]: " + to_string(it->value_) + "\n";
    }

    print += " <--- END LIST --->";

    return print;
}
