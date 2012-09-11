/*
 * Gregory Petropoulos
 *
 * This is the parallel version of the Simpson Integration Program
 *
 * To compile:  mpicxx -g -Wall -std=c99 -o parallel_simpson parallel_simpson.cpp
 * To run:  mpiexec -n 2 ./parallel_simpson [-verbose] <a> <b> <n>
 *          <> -> mandatory
 *          [] -> optional
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <iostream>
using namespace std;

// -----------------------------------------------------------------
// function we are integrating
double f_x(double x) {
  double value;

  value=2*pow(x,5)-pow(x,4)+5*pow(x,3)-10*pow(x,2)+15*x-3;
  return value;
}
// -----------------------------------------------------------------



// -----------------------------------------------------------------
// computes the volume of one element
double simpson (double local_p, double local_q, double h) {
  double integral, r;

  r=local_p+h;
  integral = (h/3) * ( f_x(local_p) + 4*f_x(r) + f_x(local_q) );

  return integral;
}
// -----------------------------------------------------------------



// -----------------------------------------------------------------
// Ends the program on an error and prints message to node 0
void cleanup (int my_rank, const char *message) {
  if (my_rank==0) {printf("%s\n",message);}
  MPI_Finalize();                                       /* kills mpi                            */
  exit(0);
}
// -----------------------------------------------------------------



// -----------------------------------------------------------------
// the main program
int main(int argc, char *argv[]) {
  double a, b, h, p, q, local_h, local_n, local_a, local_b, local_result, global_result;
  int i, n, my_rank, comm_sz;
  bool verbose = false;
  string flag;
  
  MPI_Init(&argc, &argv);                               /* start up MPI                         */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);              /* get the number of processes          */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);              /* get my rank among all the processes  */

  if (argc < 4) {                                       /* too few arguments aborts the program */
    cleanup(my_rank, "Error:  Too few arguments");
  }
  if (argc == 4) {                                      /* option to run with a b n as inputs   */
    a = strtod(argv[1], NULL);
    b = strtod(argv[2], NULL);
    n = atoi(argv[3]);
  }
  if (argc == 5) {                                      /* option to run -verbose               */
    flag = argv[1];
    if (flag.compare("-verbose") != 0) {
      cleanup(my_rank, "Error:  Wrong flag, only '-verbose' supported");
    }
    else if (flag.compare("-verbose") == 0) {           /* not optimal for one flag went with   */
      verbose = true;                                   /* general approach                     */
    }
    a = strtod(argv[2], NULL);
    b = strtod(argv[3], NULL);
    n = atoi(argv[4]);
  }
  if (argc > 5) {                                       /* too many arguments aborts the program*/
    cleanup(my_rank, "Error:  Too many arguments");
  }

  h = (b-a)/n;                                          /* computs local interval               */
  local_n = n/comm_sz;
  local_a = a+my_rank*local_n*h;
  local_b = local_a+local_n*h;

  if (local_b > b) {                                    /* issues a warning for bad [a,b] n p   */
    cout << "Warning:  A local interval on processor " <<
      my_rank << " has exceeded the integration bounds" << endl;
  }
  if ( (my_rank == (comm_sz-1)) && (local_b < b) ) {
    cout << "Warning:  A local interval on processor " <<
      my_rank << " has not reached the end of the range" << endl;
  }

  p = local_a;                                          /* calculates the integral result for   */
  q = p+(local_b-local_a)/local_n;                      /* each local process                   */
  local_h = (q-p)/2;
  local_result = 0;
  for (i = 0; i < local_n; i++) {
    local_result += simpson(p,q,local_h);
    p = q;
    q = p+(local_b-local_a)/local_n;
  }

  MPI_Reduce(&local_result, &global_result, 1,          /* sums the results across nodes and    */
      MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);          /* puts the final result in process 0   */

  if (verbose == true) {                                /* prints out a list of x values ran on */
    if (my_rank == 0) {                                 /* each processor for -verbose flag     */
      int source;

      cout << "Process 0 local_a:  " << local_a << endl;
      cout << "Process 0 local_b:  " << local_b << endl;
      cout << "Process 0 local_n:  " << local_n << endl;

      for (source = 1; source < comm_sz; source++) {
        MPI_Recv(&local_a, 1, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&local_b, 1, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&local_n, 1, MPI_DOUBLE, source, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << "Process " << source << " local_a:  " << local_a << endl;
        cout << "Process " << source << " local_b:  " << local_b << endl;
        cout << "Process " << source << " local_n:  " << local_n << endl;
      }
    }
    else {
      MPI_Send(&local_a, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
      MPI_Send(&local_b, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
      MPI_Send(&local_n, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    }
  }

  if (my_rank==0) {                                     /* outputs the answer                   */
    cout << "The integral is: " << global_result << "\n";
  }

  cleanup(my_rank, "Program Complete");                 /* terminates the program               */
  return 0;
}
// -----------------------------------------------------------------
