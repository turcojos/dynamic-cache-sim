#include <iostream>
#include <vector>

using namespace std;

class JoeCache {
  private:
    struct DirectoryEntry{
      int tag;
      int last_used;
    };

    struct JoeDirectorySet {
      int assoc;
      int access_count;
      int index;
      vector<DirectoryEntry> set_tags;
    };

    vector<JoeDirectorySet> directory;
    vector<JoeDirectorySet> sorted_directory;
    vector<int> visited_tags;
  public:
    //Cache info
    int base_assoc;
    int block_size;
    int cache_size;
    int address_width;
    int offset_length;
    int num_cache_lines;
    int num_sets;
    int index_length;
    int tag_length;
    int passiveness;
    bool write_policy;

    //statistic tracking
    int num_writes;
    int num_reads;
    int num_write_com_misses;
    int num_write_cap_misses;
    int num_write_con_misses;
    int num_read_com_misses;
    int num_read_cap_misses;
    int num_read_con_misses;
    int get_total_write_misses();
    int get_total_read_misses();
    int get_total_reqs();
    int get_total_misses();

    //cache controllers
    void write_cache(int address, int order);
    void read_cache(int address, int order);

    void is_set_full();

    bool is_cache_full();

    void emplace_entry(int index, int tag, int order);

    void rebalance();

    bool sort_helper(JoeDirectorySet i, JoeDirectorySet j);

    bool assoc_check();

    //constructor
    /*
    int assoc,
    int block_size,
    int cache_size,
    int address_width,
    bool_write_policy - true for write-allocate, false for write-through
    */
    JoeCache(int base_assoc, int block_size, int cache_size, int address_width, bool write_policy, int passiveness);

};
