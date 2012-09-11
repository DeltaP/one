/*
 * Gregory Petropoulos
 *
 * This is the serial version of the Simpson Integration Program
 *
 * To compile:  g++ -Wall -o serial_simpson serial_simpson.cpp
 * To run:  ./serial_simpson <a> <b> <n>
 *          <> -> mandatory
 *          [] -> optional
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
  integral = (h/3)*(f_x(local_p)+4*f_x(r)+f_x(local_q));

  return integral;
}
// -----------------------------------------------------------------



// -----------------------------------------------------------------
// Ends the program on an error and prints message to node 0
void cleanup (const char *message) {
  printf("%s\n",message);
  exit(0);
}
// -----------------------------------------------------------------



// -----------------------------------------------------------------
// the main program
int main(int argc, char *argv[]) {
  double a,b,p,q,h,result;
  int i,n;

  if (argc < 4) {                                       /* too few arguments aborts the program */
    cleanup("Error:  Too few arguments");
  }
  if (argc == 4) {                                      /* option to run with a b n as inputs   */
    a = strtod(argv[1], NULL);
    b = strtod(argv[2], NULL);
    n = atoi(argv[3]);
  }
  if (argc > 4) {                                       /* too many arguments aborts the program*/
    cleanup("Error:  Too many arguments");
  }

  result = 0;                                           /* calculates the integral result       */
  p = a;
  q = p+(b-a)/n;
  h = (q-p)/2;
  for (i = 0; i < n; i++) {
    result += simpson(p,q,h);
    p = q;
    q = p+(b-a)/n;
  }

  cout << "The integral is: " << result << endl;        /* outputs the answer                   */

  cleanup("Program Complete");                          /* terminates the program               */
  return 0;
}
