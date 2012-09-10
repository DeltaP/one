/*
 *This is the parallel version of the Simpson Progra
 *
 */


#include <stdio.h>
#include <mpi.h>
#include <iostream>
using namespace std;


int main(void) {
  double a,b,p,q,result;
  int i,n;
  
  MPI_Init(NULL, NULL);                                 /* Start up MPI */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);              /* Get the number of processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);              /* Get my rank among all the processes */

  if(my_rank==0) {
    //pprompt for a,b,n
    cout << "Enter the beginning of the integration interval [a,b] 'a':\n";
    cin >> a;
    cout << "Enter the end of the integration interval [a,b] 'b':\n";
    cin >> b;
    cout << "Enter the number of intervals 'n':\n";
    cin >> n;
  }

  result=0;
  p=a;
  q=p+(b-a)/n;

  return 0;
}
