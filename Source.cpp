
/*Merging sort: This program will Generate an array of integers with fixed size (16384),
and use merge sort to sort the arrays in ascending order on a variable number of processors with Hypercube (mapped as  Binary Tree).
to Compute the run time for each execution and we can use that output to calculate 
speedup and efficiency in each case.
 */
#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <fstream>
#include <cstdlib>
#include <ctime>

#define AN 16384 /* Maximum list size */
#define MAX 99  /* Maximum value of a list element */

int CUBESIZE, DIMENSION, RANK;    /* Cube size, dimension, rank */      
void parallel_mergesort(int myid, int list[], int n);
void mergesort(int list[], int left, int right, int descending);



int main(int argc, char* argv[])
{
    int list[AN], n = 16384, i;

    MPI_Init(&argc, &argv); 

    MPI_Comm_size(MPI_COMM_WORLD, &CUBESIZE);
    DIMENSION = log(CUBESIZE + 1e-10) / log(2.0);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK);

    srand((unsigned)RANK + 1);
    for (i = 0; i < n / CUBESIZE; i++) list[i] = rand() % MAX;

    printf("Before: Rank %2d :", RANK);
    for (i = 0; i < n / CUBESIZE; i++) printf("%3d ", list[i]);
    printf("\n");
    
    double start_time = MPI_Wtime(); 
    parallel_mergesort(RANK, list, n);
    double end_time = MPI_Wtime();

    printf("After:  Rank %2d :", RANK);
    for (i = 0; i < n / CUBESIZE; i++) printf("%3d ", list[i]);
    printf("\n");
    
    MPI_Finalize();
    printf("PROCESS: %d TIME: %f\n", RANK, end_time - start_time);
    
    return 0;
}




/* Sequential mergesort (either ascending or descending) */
void mergesort(int list[], int l, int r, int asc)
{
    int A, B, C, D, m, tempARRAY[AN];

    if (l < r) {
      
        m = (l + r) / 2;
       
        
        mergesort(list, l, m, asc);
        mergesort(list, m + 1, r, asc);

        C = A = l; B = m + 1;
        if (asc)
            while (A <= m && B <= r)
               
                tempARRAY[C++] = list[A] > list[B] ? list[A++] : list[B++];
       
        else
            while (A <= m && B <= r)

                tempARRAY[C++] = list[A] < list[B] ? list[A++] : list[B++];
       
        D = A > m ? B : A;

        while (C <= r) tempARRAY[C++] = list[D++];

        for (C = l; C <= r; C++) list[C] = tempARRAY[C];
    }
}
/* Parallel mergesort */
void parallel_mergesort(int myid, int list[], int n)
{
    int listSize, l, m, bitl = 1, bitm, neighbor, i;
    MPI_Status status;

    listSize = n / CUBESIZE;
    mergesort(list, 0, listSize - 1, myid & bitl);

    for (l = 1; l <= DIMENSION; l++) {
        bitl = bitl << 1;
        for (bitm = 1, m = 0; m < l - 1; m++) bitm *= 2;
        for (m = l - 1; m >= 0; m--) {
            neighbor = myid ^ bitm;
            MPI_Send(list, listSize, MPI_INT, neighbor, l * DIMENSION + m, MPI_COMM_WORLD);
            MPI_Recv(&list[listSize], listSize, MPI_INT, neighbor, l * DIMENSION + m,
                MPI_COMM_WORLD, &status);
            mergesort(list, 0, 2 * listSize - 1, myid & bitl);
            if (myid & bitm)
                for (i = 0; i < listSize; i++) list[i] = list[i + listSize];
            bitm = bitm >> 1;
        }
    }
}