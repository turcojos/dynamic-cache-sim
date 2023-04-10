#include "BasicCache.h"
#include <vector>
#include <cmath>

using namespace std;

//Constructor
BasicCache::BasicCache(int assoc, int block_size, int cache_size, int address_width) {
  this->assoc = assoc;
  this->block_size = block_size;
  this->cache_size = cache_size;
  this->address_width = address_width;

  this->offset_length = (int)log2(block_size);
  this->num_cache_lines = (int)(cache_size/block_size);
  this->num_sets = (int)(num_cache_lines/assoc);
  this->index_length = (int)log2(num_sets);
  this->tag_length = address_width - index_length - offset_length;

  for (int i = 0; i < num_sets; i++) {
    BasicDirectorySet d;
    for(int j = 0; j < assoc; j++) {
      DirectoryEntry e;
      e.tag=NULL;
      e.last_used=0;
      d.set_tags.push_back(e);
    }
    directory.push_back(d);
  }
}
