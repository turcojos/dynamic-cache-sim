#include <iostream>
#include <random>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

int main() {
  int length;
  char resp = 'X';
  while (resp != 'S' && resp != 'M' && resp != 'L') {
    cout << "How large of a tracefile (S, M, L)? ";
    cin >> resp;
  }

  if (resp == 'S') {
    length = 10000;
  }
  else if (resp == 'M') {
    length = 100000;
  }
  else {
    length = 1000000;
  }

  resp = 'X';
  while (resp != 'N' && resp != 'U') {
    cout << "Non-uniform or Uniform (N, U)? ";
    cin >> resp;
  }

  int center;
  int stdev;

  if (resp == 'N') {
    cout << "Average Address: ";
    cin >> center;

    cout << "Standard Deviation: ";
    cin >> stdev;
  }

  string filename;
  cout << "Enter filepath: ";
  cin >> filename;

  ofstream ofile;
  ofile.open(filename);

  vector<int> written;

  default_random_engine generator;

  switch (resp) {
    case 'N':
      {
        normal_distribution<double> n_dist(center, stdev);
        int addr;
        for(int i = 0; i < length; i++) {
          addr = n_dist(generator);
          if (addr < 0) {
            addr = 0;
          }
          if (addr > 65535) {
            addr = 65535;
          }
          if (find(written.begin(), written.end(), addr) == written.end()) {
            ofile << "W" << " " << addr << "\n";
            written.push_back(addr);
          }
          else {
            ofile << "R" << " " << addr << "\n";
            ofile << "W" << " " << addr << "\n";
            ofile << "R" << " " << addr << "\n";
          }
        }
        ofile.close();
      }
      break;
    case 'U':
      {
        uniform_real_distribution<double> n_dist(0, 65535);
        int addr;
        for(int i = 0; i < length; i++) {
          addr = n_dist(generator);
          if (find(written.begin(), written.end(), addr) == written.end()) {
            ofile << "W" << " " << addr << "\n";
            written.push_back(addr);
          }
          else {
            ofile << "R" << " " << addr << "\n";
          }
        }
        ofile.close();
      }
      break;
  }
}
