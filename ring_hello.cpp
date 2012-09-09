/*
 * Gregory Petropoulos
 *
 * Ring Hello World Program
 * To compile:  mpicxx -g -Wall -std=c99 -o ring_hello ring_hello.cpp
 * To run:  mpiexec -n 4 ./ring_hello
 */

#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(void) {
  int        comm_sz;                                   /* Number of processes    */
  int        my_rank;                                   /* My process rank        */
  char message[MAX_STRING];

  
  MPI_Init(NULL, NULL);                                 /* Start up MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);              /* Get the number of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);              /* Get my rank among all the processes */

  int to   = (my_rank+1)%comm_sz;
  int from = (my_rank-1+comm_sz)%comm_sz;
  sprintf(message, "Greetings from process %d of %d", my_rank, comm_sz);

  MPI_Send(message, strlen(message)+1, MPI_CHAR, to, 0, MPI_COMM_WORLD);
  MPI_Recv(message, MAX_STRING, MPI_CHAR, from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  printf("%s | reported by process %d of %d\n", message, my_rank, comm_sz);
    
  MPI_Finalize();
  return 0;
}
