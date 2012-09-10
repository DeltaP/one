/*
 * This is the serial version of the Simpson Integration Program
 *
 * compile:  g++ -Wall -o serial_simpson serial_simpson.cpp
 */

#include <stdio.h>
#include <iostream>
using namespace std;

double f_x(double x) {
  double value;

  value=2*x*x+3*x-15;
  return value;
}

double simpson (double local_p, double local_q) {
  double integral, h, r;

  h=(local_q-local_p)/2;
  r=local_p+h;
  integral = (h/3)*(f_x(local_p)+4*f_x(r)+f_x(local_q));

  return integral;
}

int main(void) {
  double a,b,p,q,result;
  int i,n;
  
  a=0;
  b=100;
  n=100;
  result=0;

  p=a;
  q=p+(b-a)/n;
  for(i=0; i<n; i++) {
    result+=simpson(p,q);
    p=q;
    q=p+(b-a)/n;
  }

  cout << "The integral is: " << result << "\n";

  return 0;
}
