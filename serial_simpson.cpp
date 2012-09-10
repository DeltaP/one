/*
 * This is the serial version of the Simpson Integration Program
 *
 * To compile:  g++ -Wall -o serial_simpson serial_simpson.cpp
 * To run:  ./serial_simpson
 */

#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;

double f_x(double x) {
  double value;

  value=2*pow(x,3)+3*pow(x,2)+x-15;
  return value;
}

double simpson (double local_p, double local_q, double h) {
  double integral, r;

  r=local_p+h;
  integral = (h/3)*(f_x(local_p)+4*f_x(r)+f_x(local_q));

  return integral;
}

int main(void) {
  double a,b,p,q,h,result;
  int i,n;
  
  //prompt for a,b,n
  cout << "Enter the beginning of the integration interval [a,b] 'a':\n";
  cin >> a;
  cout << "Enter the end of the integration interval [a,b] 'b':\n";
  cin >> b;
  cout << "Enter the number of intervals 'n':\n";
  cin >> n;

  result=0;
  p=a;
  q=p+(b-a)/n;
  h=(q-p)/2;
  for(i=0; i<n; i++) {
    result+=simpson(p,q,h);
    p=q;
    q=p+(b-a)/n;
  }

  cout << "The integral is: " << result << "\n";

  return 0;
}
