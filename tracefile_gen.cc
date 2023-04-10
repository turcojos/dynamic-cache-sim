#include <iostream>
#include <random>

using namesspace std;

int main() {
  int length;
  char resp = 'X';
  while (resp != 'S' && resp != 'M' && resp != 'L') {
    cout << "How large of a tracefile (S, M, L)? "
    cin >> resp;
  }

  if (resp == 'S') {
    length = 1000;
  }
  else if (resp == 'M') {
    length = 10000;
  }
  else {
    length = 100000;
  }

  char resp = 'X';
  while (resp != 'N' && resp != 'U') {
    cout << "Non-uniform or Uniform (N, U)? "
    cin >> resp;
  }

  string filename;
  cout << "Enter filename: "
  cin >> filename;

  switch (resp) {
    case 'N':
         default_random_engine generator;
         normal_distribution<double> u_dist(32000, 10000);
         int addr;
         for(int i = 0; i < length; i++) {
           addr = u_dist(generator);
           if (addr < 0) {
             addr = 0;
           }
           if (addr > 65535) {
             addr = 65535;
           }
         }
      break;
    case 'U':
      break;
  }
}
