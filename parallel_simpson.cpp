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

// -----------------------------------------------------------------
// function we are integrating
double f_x(double x) {
  double value;

  value=2*pow(x,5)-15*pow(x,3);
  return value;
}
// -----------------------------------------------------------------



// -----------------------------------------------------------------
// computes the volume of one element
double simpson (double local_p, double local_q, double h) {
  double integral, r;

  r=local_p+h;
  integral = (h/3)*(f_x(local_p)+4*f_x(r)+f_x(local_q));

  return integral;
}
// -----------------------------------------------------------------



// -----------------------------------------------------------------
// the main program
int main(void) {
  double p, q, local_h, local_n, local_a, local_b, local_result, global_result;
  int i, my_rank, comm_sz;
  double* a = (double*) malloc(sizeof(double));
  double* b = (double*) malloc(sizeof(double));
  double* h = (double*) malloc(sizeof(double));
  int*    n = (int*)    malloc(sizeof(int));
  
  MPI_Init(NULL, NULL);                                 /* start up MPI                         */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);              /* get the number of processes          */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);              /* get my rank among all the processes  */

  if(my_rank==0) {                                      /* prompts for a, b, n and computes h   */
    cout << "Enter the beginning of the integration interval [a,b] 'a':\n";
    cin >> *a;
    cout << "Enter the end of the integration interval [a,b] 'b':\n";
    cin >> *b;
    cout << "Enter the number of intervals 'n':\n";
    cin >> *n;
    *h=(*b-*a)/(*n);
  }

  MPI_Bcast(a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);       /* broadcast inputs to all nodes        */
  MPI_Bcast(b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(h, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(n, 1, MPI_INT,    0, MPI_COMM_WORLD);

  local_n = *n/comm_sz;
  local_a = *a+my_rank*local_n*(*h);
  local_b = local_a+local_n*(*h);

  cout << "process " << my_rank << " local_n: "         /* verbose output needs to be           */
    << local_n << "\n";                                 /* taken care of                        */
  cout << "process " << my_rank << " local_a: "
    << local_a << "\n";
  cout << "process " << my_rank << " local_b: "
    << local_b << "\n";

  p = local_a;                                          /* calculates the integral result for   */
  q = p+(local_b-local_a)/local_n;                      /* each local process                   */
  local_h = (q-p)/2;
  local_result = 0;
  for(i=0; i<local_n; i++) {
    local_result+=simpson(p,q,local_h);
    p=q;
    q=p+(local_b-local_a)/local_n;
  }

  MPI_Reduce(&local_result, &global_result, 1,          /* sums the results across nodes and    */
      MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);          /* puts the final result in process 0   */

  if (my_rank==0) {                                     /* outputs the answer                   */
    cout << "The integral is: " << global_result << "\n";
  }

  MPI_Finalize();                                       /* kills mpi                            */
  return 0;
}
// -----------------------------------------------------------------

/*
 * questions to ask:
 * can I calculate local_h each time, this elevitaes problem of odd divisors
 *
 * do i have to message pass the boundaries?
 *
 * thigns to do:  add -verbose and output what is wanted on the handout
 */
