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
#include <string>
#include <sstream>
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
// computes the volume of one element
double simpson (double local_a, int n, double local_h, int count, bool v, string& out) {
  double integral, x;
  int coefficient;

  x = local_a + count * local_h;

  if ( (count == 0) || (count == n) ) {
    coefficient = 1;
  }
  else if (count % 2 == 0) {
    coefficient = 2;
  }
  else {
    coefficient = 4;
  }

  integral = (local_h/3)*coefficient*f_x(x);

  if (v == true) {
    ostringstream convert;
    convert << coefficient;
    out = out + convert.str() + " ";
  }

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
  double a, b, h, result;
  int i, n;
  bool verbose = false;
  string flag, out;

  if (argc < 4) {                                       /* too few arguments aborts the program */
    cleanup("Error:  Too few arguments");
  }
  if (argc == 4) {                                      /* option to run with a b n as inputs   */
    a = strtod(argv[1], NULL);
    b = strtod(argv[2], NULL);
    n = atoi(argv[3]);
  }
  if (argc == 5) {                                      /* option to run with a b n as inputs   */
    flag = argv[1];
    if (flag.compare("-verbose") != 0) {
      cleanup("Error:  Wrong flag, only '-verbose' supported");
    }
    else if (flag.compare("-verbose") == 0) {           /* not optimal for one flag went with   */
      verbose = true;                                   /* general approach                     */
    }
    a = strtod(argv[2], NULL);
    b = strtod(argv[3], NULL);
    n = atoi(argv[4]);
  }
  if (argc > 5) {                                       /* too many arguments aborts the program*/
    cleanup("Error:  Too many arguments");
  }

  result = 0;                                           /* calculates the integral result       */
  h = (b-a)/n;
  for (i = 0; i < n+1; i++) {
    result += simpson(a, n, h, i, verbose, out);
  }

  if (verbose == true) { cout << out << endl; }
  cout << "The integral is: " << result << endl;        /* outputs the answer                   */

  cleanup("Program Complete");                          /* terminates the program               */
  return 0;
}
