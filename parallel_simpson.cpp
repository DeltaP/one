/*
 * This is the parallel version of the Simpson Progra
 * To compile:  mpicxx -g -Wall -std=c99 -o parallel_simpson parallel_simpson.cpp
 * To run:  mpiexec -n 2 ./parallel_simpson
 */

#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <iostream>
using namespace std;

double f_x(double x) {
  double value;

  value=2*pow(x,5)-15*pow(x,3);
  return value;
}

double simpson (double local_p, double local_q, double h) {
  double integral, r;

  r=local_p+h;
  integral = (h/3)*(f_x(local_p)+4*f_x(r)+f_x(local_q));

  return integral;
}

int main(void) {
  double p, q, local_h, local_n, local_a, local_b, local_result, global_result;
  int i, my_rank, comm_sz;
  double* a = (double*) malloc(sizeof(double));
  double* b = (double*) malloc(sizeof(double));
  double* h = (double*) malloc(sizeof(double));
  int*    n = (int*)    malloc(sizeof(int));
  
  MPI_Init(NULL, NULL);                                 /* Start up MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);              /* Get the number of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);              /* Get my rank among all the processes */

  if(my_rank==0) {
    //pprompt for a,b,n
    cout << "Enter the beginning of the integration interval [a,b] 'a':\n";
    cin >> *a;
    cout << "Enter the end of the integration interval [a,b] 'b':\n";
    cin >> *b;
    cout << "Enter the number of intervals 'n':\n";
    cin >> *n;
    *h=(*b-*a)/(*n);
  }

  MPI_Bcast(a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(h, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(n, 1, MPI_INT, 0, MPI_COMM_WORLD);


  cout << "process " << my_rank << " h: " << *h << "\n";
  local_n=*n/comm_sz;
  cout << "process " << my_rank << " local_n: " << local_n << "\n";
  local_a=*a+my_rank*local_n*(*h);
  cout << "process " << my_rank << " local_a: " << local_a << "\n";
  local_b=local_a+local_n*(*h);
  cout << "process " << my_rank << " local_b: " << local_b << "\n";

  local_result=0;
  p=local_a;
  q=p+(local_b-local_a)/local_n;
  local_h=(q-p)/2;
  for(i=0; i<local_n; i++) {
    local_result+=simpson(p,q,local_h);
    p=q;
    q=p+(local_b-local_a)/local_n;
  }

  MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (my_rank==0) {
    cout << "The integral is: " << global_result << "\n";
  }

  MPI_Finalize();
  return 0;
}
