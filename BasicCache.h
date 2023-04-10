#include <iostream>
#include <vector>

using namespace std;

class BasicCache {
  private:
    struct DirectoryEntry{
      int tag;
      int last_used;
    };

    struct BasicDirectorySet {
      vector<DirectoryEntry> set_tags;
    };

    vector<BasicDirectorySet> directory;
  public:
    //Cache info
    int assoc;
    int block_size;
    int cache_size;
    int address_width;

    int offset_length;
    int num_cache_lines;
    int num_sets;
    int index_length;
    int tag_length;

    BasicCache(int assoc, int block_size, int cache_size, int address_width);

};
