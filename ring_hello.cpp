/*
 * Gregory Petropoulos
 *
 * Ring Hello World Program
 * To compile:  mpicxx -g -Wall -std=c99 -o ring_hello ring_hello.cpp
 * To run:  mpiexec -n 4 ./ring_hello
 */

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <mpi.h>
using namespace std;

const int MAX_STRING = 100;

int main(void) {
  int  comm_sz;                                          /* Number of processes                 */
  int  my_rank;                                          /* My process rank                     */
  char send[MAX_STRING];
  char message[MAX_STRING];

  
  MPI_Init(NULL, NULL);                                  /* Start up MPI                        */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);               /* Get the number of processes         */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);               /* Get my rank among all the processes */

  int to   = (my_rank+1)%comm_sz;                        /* finds which process to send to      */
  int from = (my_rank-1+comm_sz)%comm_sz;                /* finds which process to recieve from */
  sprintf(send, "Greetings from process %d of %d",       /* forms message                       */
      my_rank, comm_sz);

  if (comm_sz == 1) {
    cout << "No need to pass messages; only one process.\n";
    MPI_Finalize();
    exit(0);
  }
  else if (my_rank%2 == 0) {                             /* sends even numbers first            */
  MPI_Send(send, strlen(send)+1, MPI_CHAR, to, my_rank,  /* sends mpi message                   */
      MPI_COMM_WORLD);
  MPI_Recv(message, MAX_STRING, MPI_CHAR, from, from,    /* recieves mpi message                */
      MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  else {                                                 /* recieves odd numbers first          */
  MPI_Recv(message, MAX_STRING, MPI_CHAR, from, from,    /* recieves mpi message                */
      MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Send(send, strlen(send)+1, MPI_CHAR, to, my_rank,  /* sends mpi message                   */
      MPI_COMM_WORLD);
  }

  printf("%s | reported by process %d of %d\n",          /* prints output                       */
      message, my_rank, comm_sz);
    
  MPI_Finalize();                                        /* kills mpi                           */
  return 0;
}
