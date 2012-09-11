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
#include <sstream>
#include <string>
using namespace std;

// -----------------------------------------------------------------
// function we are integrating
double f_x(double x) {
  double value;

  //value=2*pow(x,5)-pow(x,4)+5*pow(x,3)-10*pow(x,2)+15*x-3;
  value = pow(x,1);
  return value;
}
// -----------------------------------------------------------------



// -----------------------------------------------------------------
// performs the sum requird by the simpson rule
double simpson (double a, double n, double h, int count, bool v, string& out) {
  double integral, x;
  int coefficient;

  if ( (count == 0) || (count == n) ) { coefficient = 1; }
  else if (count % 2 == 0)            { coefficient = 2; }
  else                                { coefficient = 4; }

  x = a + count * h;
  integral = (h/3)*coefficient*f_x(x);

  if (v == true) {
    ostringstream convert;
    convert << coefficient;
    out += convert.str() + " ";
  }

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
  double a, b, h, local_result, global_result;
  int i, n, local_n, my_rank, comm_sz;
  bool verbose = false;
  string flag, out;
  
  MPI_Init(&argc, &argv);                               /* start up MPI                         */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);              /* get the number of processes          */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);              /* get my rank among all the processes  */

  if (argc < 4) {                                       /* too few arguments aborts the program */
    cleanup(my_rank, "Error:  Too few arguments");
  }
  else if (argc == 4) {                                      /* option to run with a b n as inputs   */
    a = strtod(argv[1], NULL);
    b = strtod(argv[2], NULL);
    n = atoi(argv[3]);
  }
  else if (argc == 5) {                                      /* option to run -verbose               */
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
  else if (argc > 5) {                                       /* too many arguments aborts the program*/
    cleanup(my_rank, "Error:  Too many arguments");
  }

  if (n%comm_sz !=0) {
    cleanup(my_rank, "Error:  The provided n does not divide by the number of processors");
  }

  h = (b-a)/n;                                          /* computs local interval               */
  local_n = n/comm_sz;
  local_result = 0;
  if ( my_rank < comm_sz-1 ) {
    for (i = local_n*my_rank; i < my_rank*local_n+local_n; i++) {
      local_result += simpson(a, n, h, i, verbose, out);
    }
  }
  else {
    for (i = local_n*my_rank; i < my_rank*local_n+local_n+1; i++) {
      local_result += simpson(a, n, h, i, verbose, out);
    }
  }

  MPI_Reduce(&local_result, &global_result, 1,          /* sums the results across nodes and    */
      MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);          /* puts the final result in process 0   */

  if (verbose == true) { cout << "Process " << my_rank << ":  " << out << endl; }

  if (my_rank==0) {                                     /* outputs the answer                   */
    cout << "The integral is: " << global_result << endl;
  }

  cleanup(my_rank, "Program Complete");                 /* terminates the program               */
  return 0;
}
// -----------------------------------------------------------------
