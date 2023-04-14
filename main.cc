#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include "BasicCache.h"
#include "WSDAMCache.h"
#include "JoeCache.h"

//int assoc = 2; // Associativity of the cache
int block_size = 32; //Size of the block in bytes
//int cache_size = 8192; // size of the cache in bytes
int address_width = 16; // size of the address in bits
int tracefile_opt = 0;

using namespace std;

int main(int argc, char *argv[]) {
  //ifstream ifile;
  ofstream ofile;

  string filename = argv[1];

  ofile.open(filename + "_out_data");


  //Cache info
  /*
  int offset_length = (int)log2(blocksize);
  int num_cache_lines = (int)(cache_size/block_size);
  int num_sets = (int)(num_cache_lines/assoc);
  int index_length = (int)log2(num_sets);
  int tag_width = address_width - index_length - offset_length;
  */
  for(int assoc = 2; assoc < 16; assoc*=2) {
    for(int cache_size = 2048; cache_size < 2048*32; cache_size*=2) {
      ifstream ifile;
      ifile.open(filename);


      BasicCache cache_1(assoc, block_size, cache_size, address_width, true);
      WSDAMCache cache_2(assoc, block_size, cache_size, address_width, true, 5000);
      WSDAMCache cache_3(assoc, block_size, cache_size, address_width, true, 10000);
      JoeCache cache_4(assoc, block_size, cache_size, address_width, true, 10000);

      char type;
      int address;
      int tag;
      int index;
      int offset;

      int order = 0;

      while(ifile) {
        ifile >> type;
        ifile >> address;

        order++;

        offset = address & ((int)pow(2, cache_1.offset_length) - 1);
        index = address & (((int)pow(2, cache_1.index_length) - 1) << cache_1.offset_length);
        tag = address & (((int)pow(2, cache_1.tag_length) - 1) << (cache_1.offset_length+cache_1.index_length));
        if (type == 'W') {
          cache_1.write_cache(address,order);
          cache_2.write_cache(address,order);
          cache_3.write_cache(address,order);
          cache_4.write_cache(address,order);
        }
        else {
          cache_1.read_cache(address,order);
          cache_2.read_cache(address,order);
          cache_3.read_cache(address,order);
          cache_4.read_cache(address,order);
        }
      }

      cout << "=================================================================================\n";
      cout << "Assoc: "<< assoc << " Block_size: " << block_size << " Cache_size: " << cache_size <<"\n";


      cout << "Basic Cache :" << endl;
      cout << "Total writes: " << cache_1.num_writes << endl;
      cout << "Total reads: " << cache_1.num_reads << endl;

      cout << "Total write misses: " << cache_1.get_total_write_misses() << " Comp: "<<cache_1.num_write_com_misses <<" Conf: "<<cache_1.num_write_con_misses << " Capa: "<<cache_1.num_write_cap_misses << endl;
      cout << "Total read misses: " << cache_1.get_total_read_misses() << " Comp: "<<cache_1.num_read_com_misses << " Conf: "<<cache_1.num_read_con_misses << " Capa: "<<cache_1.num_read_cap_misses << endl;

      cout << "Total misses: " << cache_1.get_total_read_misses() + cache_1.get_total_write_misses()<< endl<<endl;

      cout << "Normal Dynamic Cache :" << endl;

      cout << "Valid: " << cache_2.assoc_check()<<endl;

      cout << "Total writes: " << cache_2.num_writes << endl;
      cout << "Total reads: " << cache_2.num_reads << endl;

      cout << "Total write misses: " << cache_2.get_total_write_misses() << " Comp: "<<cache_2.num_write_com_misses << " Conf: "<<cache_2.num_write_con_misses << " Capa: "<<cache_2.num_write_cap_misses << endl;
      cout << "Total read misses: " << cache_2.get_total_read_misses() << " Comp: "<<cache_2.num_read_com_misses << " Conf: "<<cache_2.num_read_con_misses << " Capa: "<<cache_2.num_read_cap_misses << endl;
      cout << "Total misses: " << cache_2.get_total_read_misses() + cache_2.get_total_write_misses()<< endl<<endl;


      cout << "Aggressive Dynamic Cache :" << endl;

      cout << "Valid: " << cache_3.assoc_check()<<endl;

      cout << "Total writes: " << cache_3.num_writes << endl;
      cout << "Total reads: " << cache_3.num_reads << endl;

      cout << "Total write misses: " << cache_3.get_total_write_misses() << " Comp: "<<cache_3.num_write_com_misses << " Conf: "<<cache_3.num_write_con_misses <<" Capa: "<<cache_3.num_write_cap_misses << endl;
      cout << "Total read misses: " << cache_3.get_total_read_misses() << " Comp: "<<cache_3.num_read_com_misses <<" Conf: "<<cache_3.num_read_con_misses << " Capa: "<<cache_3.num_read_cap_misses << endl;
      cout << "Total misses: " << cache_3.get_total_read_misses() + cache_3.get_total_write_misses()<< endl <<endl;

      cout << "Hyper Specialized Dynamic Cache :" << endl;

      cout << "Valid: " << cache_4.assoc_check()<<endl;

      cout << "Total writes: " << cache_4.num_writes << endl;
      cout << "Total reads: " << cache_4.num_reads << endl;

      cout << "Total write misses: " << cache_4.get_total_write_misses() << " Comp: "<<cache_4.num_write_com_misses << " Conf: "<<cache_4.num_write_con_misses << " Capa: "<<cache_4.num_write_cap_misses << endl;
      cout << "Total read misses: " << cache_4.get_total_read_misses() << " Comp: "<<cache_4.num_read_com_misses << " Conf: "<<cache_4.num_read_con_misses << " Capa: "<<cache_4.num_read_cap_misses << endl;
      cout << "Total misses: " << cache_4.get_total_read_misses() + cache_4.get_total_write_misses()<< endl <<endl;

      ofile << "=================================================================================\n";
      ofile << "Assoc: "<< assoc << " Block_size: " << block_size << " Cache_size: " << cache_size <<"\n";


      ofile << "Basic Cache :" << endl;
      ofile << "Total writes: " << cache_1.num_writes << endl;
      ofile << "Total reads: " << cache_1.num_reads << endl;

      ofile << "Total write misses: " << cache_1.get_total_write_misses() << " Comp: "<<cache_1.num_write_com_misses <<" Conf: "<<cache_1.num_write_con_misses << " Capa: "<<cache_1.num_write_cap_misses << endl;
      ofile << "Total read misses: " << cache_1.get_total_read_misses() << " Comp: "<<cache_1.num_read_com_misses << " Conf: "<<cache_1.num_read_con_misses << " Capa: "<<cache_1.num_read_cap_misses << endl;

      ofile << "Total misses: " << cache_1.get_total_read_misses() + cache_1.get_total_write_misses()<< endl<<endl;

      ofile << "Normal Dynamic Cache :" << endl;

      ofile << "Valid: " << cache_2.assoc_check()<<endl;

      ofile << "Total writes: " << cache_2.num_writes << endl;
      ofile << "Total reads: " << cache_2.num_reads << endl;

      ofile << "Total write misses: " << cache_2.get_total_write_misses() << " Comp: "<<cache_2.num_write_com_misses << " Conf: "<<cache_2.num_write_con_misses << " Capa: "<<cache_2.num_write_cap_misses << endl;
      ofile << "Total read misses: " << cache_2.get_total_read_misses() << " Comp: "<<cache_2.num_read_com_misses << " Conf: "<<cache_2.num_read_con_misses << " Capa: "<<cache_2.num_read_cap_misses << endl;
      ofile << "Total misses: " << cache_2.get_total_read_misses() + cache_2.get_total_write_misses()<< endl<<endl;


      ofile << "Aggressive Dynamic Cache :" << endl;

      ofile << "Valid: " << cache_3.assoc_check()<<endl;

      ofile << "Total writes: " << cache_3.num_writes << endl;
      ofile << "Total reads: " << cache_3.num_reads << endl;

      ofile << "Total write misses: " << cache_3.get_total_write_misses() << " Comp: "<<cache_3.num_write_com_misses << " Conf: "<<cache_3.num_write_con_misses <<" Capa: "<<cache_3.num_write_cap_misses << endl;
      ofile << "Total read misses: " << cache_3.get_total_read_misses() << " Comp: "<<cache_3.num_read_com_misses <<" Conf: "<<cache_3.num_read_con_misses << " Capa: "<<cache_3.num_read_cap_misses << endl;
      ofile << "Total misses: " << cache_3.get_total_read_misses() + cache_3.get_total_write_misses()<< endl <<endl;

      ofile << "Hyper Specialized Dynamic Cache :" << endl;

      ofile << "Valid: " << cache_4.assoc_check()<<endl;

      ofile << "Total writes: " << cache_4.num_writes << endl;
      ofile << "Total reads: " << cache_4.num_reads << endl;

      ofile << "Total write misses: " << cache_4.get_total_write_misses() << " Comp: "<<cache_4.num_write_com_misses << " Conf: "<<cache_4.num_write_con_misses << " Capa: "<<cache_4.num_write_cap_misses << endl;
      ofile << "Total read misses: " << cache_4.get_total_read_misses() << " Comp: "<<cache_4.num_read_com_misses << " Conf: "<<cache_4.num_read_con_misses << " Capa: "<<cache_4.num_read_cap_misses << endl;
      ofile << "Total misses: " << cache_4.get_total_read_misses() + cache_4.get_total_write_misses()<< endl <<endl;
    }
  }

  return 0;
}
