#include <iostream>
#include <cmath>
#include <fstream>
#include "BasicCache.h"

int assoc = 2; // Associativity of the cache
int block_size = 32; //Size of the block in bytes
int cache_size = 512; // size of the cache in bytes
int address_width = 16; // size of the address in bits
int tracefile_opt = 0;

using namespace std;

int main() {
  ifstream ifile;

  switch(tracefile_opt) {
    case 0:
      ifile.open("random_uniform_dist_trace");
      break;
  }

  //Cache info
  /*
  int offset_length = (int)log2(blocksize);
  int num_cache_lines = (int)(cache_size/block_size);
  int num_sets = (int)(num_cache_lines/assoc);
  int index_length = (int)log2(num_sets);
  int tag_width = address_width - index_length - offset_length;
  */

  BasicCache cache_1(assoc, block_size, cache_size, address_width);

  char type;
  int address;
  int tag;
  int index;
  int offset;

  address = 0b1000000101011111;

  offset = address & ((int)pow(2, cache_1.offset_length) - 1);
  index = (address & (((int)pow(2, cache_1.index_length) - 1)) << cache_1.offset_length) >> cache_1.offset_length;
  tag = (address & (((int)pow(2, cache_1.tag_length) - 1)) << (cache_1.offset_length+cache_1.index_length)) >> (cache_1.offset_length+cache_1.index_length);

  cout << offset << " " << index << " " << tag;

  while(ifile) {
    ifile >> type;
    ifile >> address;

    offset = address & ((int)pow(2, cache_1.offset_length) - 1);
    index = address & (((int)pow(2, cache_1.index_length) - 1) << cache_1.offset_length);
    tag = address & (((int)pow(2, cache_1.tag_length) - 1) << (cache_1.offset_length+cache_1.index_length));
    if (type == 'W') {

    }
    else {

    }
  }

  return 0;
}
