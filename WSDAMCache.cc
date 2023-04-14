#include "WSDAMCache.h"
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

//Constructor
WSDAMCache::WSDAMCache(int base_assoc, int block_size, int cache_size, int address_width, bool write_policy, int passiveness) {
  this->base_assoc = base_assoc;
  this->block_size = block_size;
  this->cache_size = cache_size;
  this->address_width = address_width;
  this->write_policy = write_policy;
  this->passiveness = passiveness;

  this->offset_length = (int)log2(block_size);
  this->num_cache_lines = (int)(cache_size/block_size);
  this->num_sets = (int)(num_cache_lines/base_assoc);
  this->index_length = (int)log2(num_sets);
  this->tag_length = address_width - index_length - offset_length;

  num_writes = 0;
  num_reads = 0;
  num_write_com_misses = 0;
  num_write_cap_misses = 0;
  num_write_con_misses = 0;
  num_read_com_misses = 0;
  num_read_cap_misses = 0;
  num_read_con_misses = 0;

  for (int i = 0; i < num_sets; i++) {
    WSDAMDirectorySet d;
    d.assoc = base_assoc;
    d.access_count = 0;
    d.index = i;
    for(int j = 0; j < d.assoc; j++) {
      DirectoryEntry e;
      e.tag=-1;
      e.last_used=0;
      d.set_tags.push_back(e);
    }
    directory.push_back(d);
  }
}

//write misses total
int WSDAMCache::get_total_write_misses() {
  return num_write_com_misses + num_write_cap_misses + num_write_con_misses;
}

int WSDAMCache::get_total_read_misses() {
  return num_read_com_misses + num_read_cap_misses + num_read_con_misses;
}

int WSDAMCache::get_total_reqs() {
  return num_writes + num_reads;
}
int WSDAMCache::get_total_misses() {
  return get_total_read_misses() + get_total_write_misses();
}

//cache controllers
void WSDAMCache::write_cache(int address, int order) {
  int offset = address & ((int)pow(2, this->offset_length) - 1);
  int index = (address & (((int)pow(2, this->index_length) - 1)) << this->offset_length) >> this->offset_length;
  int tag = (address & (((int)pow(2, this->tag_length) - 1)) << (this->offset_length+this->index_length)) >> (this->offset_length+this->index_length);

  num_writes++;
  directory.at(index).access_count++;
  WSDAMDirectorySet d = directory.at(index);
  for(int i = 0; i < d.assoc; i++) {
    if (d.set_tags.at(i).tag == tag) {
      return;
    }
  }


  if (is_cache_full()) {
    num_write_cap_misses++;
  }
  else if (find(visited_tags.begin(), visited_tags.end(), address) == visited_tags.end()) {
    num_write_com_misses++;

  }
  else {
    num_write_con_misses++;
  }

  if (this->write_policy) {
    emplace_entry(index, tag, order);

  }
  if(order % this->passiveness == 0) {
    rebalance();
  }

}

void WSDAMCache::read_cache(int address, int order) {
  int offset = address & ((int)pow(2, this->offset_length) - 1);
  int index = (address & (((int)pow(2, this->index_length) - 1)) << this->offset_length) >> this->offset_length;
  int tag = (address & (((int)pow(2, this->tag_length) - 1)) << (this->offset_length+this->index_length)) >> (this->offset_length+this->index_length);

  num_reads++;
  directory.at(index).access_count++;
  WSDAMDirectorySet d = directory.at(index);
  for(int i = 0; i < d.assoc; i++) {
    if (d.set_tags.at(i).tag == tag) {
      return;
    }
  }


  if (is_cache_full()) {
    num_read_cap_misses++;
  }
  else if (find(visited_tags.begin(), visited_tags.end(), tag) == visited_tags.end()) {
    num_read_com_misses++;
    visited_tags.push_back(tag);
  }
  else {
    num_read_con_misses++;
  }

  emplace_entry(index, tag, order);

  if(order % this->passiveness == 0) {
    rebalance();
  }
}



bool WSDAMCache::is_cache_full() {
  for(int i = 0; i < directory.size(); i++) {
    for(int j = 0; j < directory.at(i).set_tags.size(); j++) {
      if(directory.at(i).set_tags.at(j).tag == -1) {
        return false;
      }
    }
  }
  return true;
}

void WSDAMCache::emplace_entry(int index, int tag, int order) {
  int oldest_index = 0;
  int oldest_counter = 99999;
  for(int i = 0; i < directory.at(index).assoc; i++) {
    if(directory.at(index).set_tags.at(i).tag == -1) {
      oldest_index = i;
    }
    if(directory.at(index).set_tags.at(i).last_used < oldest_counter) {
      oldest_counter = directory.at(index).set_tags.at(i).last_used;
      oldest_index = i;
    }
  }
  directory.at(index).set_tags.at(oldest_index).tag = tag;
  directory.at(index).set_tags.at(oldest_index).last_used = order;
}

void WSDAMCache::rebalance() {
  this->sorted_directory = directory;
  sort(sorted_directory.begin(), sorted_directory.end(), [](WSDAMDirectorySet a, WSDAMDirectorySet b) {return a.access_count>b.access_count;});
  DirectoryEntry ent;
  ent.tag = -1;
  ent.last_used = 0;

  for(int i = 0; i <=num_sets/4 - 1; i++) {
    directory.at(sorted_directory.at(i).index).assoc = base_assoc*2;
    directory.at(sorted_directory.at(i).index).set_tags.resize(base_assoc*2, ent);
  }

  for(int i = num_sets/4; i <=2*num_sets/4 - 1; i++) {
    directory.at(sorted_directory.at(i).index).assoc = base_assoc;
    directory.at(sorted_directory.at(i).index).set_tags.resize(base_assoc, ent);
  }

  for(int i = 2*num_sets/4; i < num_sets; i++) {
    directory.at(sorted_directory.at(i).index).assoc = base_assoc/2;
    directory.at(sorted_directory.at(i).index).set_tags.resize(base_assoc/2, ent);
  }
  /*
  for(int i = 0; i < directory.size(); i++) {
    if(i == least_index || i == second_least_index) {
      if(directory.at(i).assoc != base_assoc/2) {
        directory.at(i).assoc = base_assoc/2;
        directory.at(i).set_tags.resize(base_assoc/2, set);
      }
    }
    else if (i == most_index) {
      if(directory.at(i).assoc != base_assoc*2) {
        directory.at(i).assoc = base_assoc*2;
        directory.at(i).set_tags.resize(base_assoc*2, set);
      }
    }
    else {
      directory.at(i).assoc = base_assoc;
      directory.at(i).set_tags.resize(base_assoc, set);
    }
  }
  */
}

bool WSDAMCache::assoc_check (){
  int sum = 0;
  for(int i = 0; i < directory.size(); i++) {
    sum = sum +directory.at(i).assoc;
  }
  return sum == num_sets*base_assoc;
}

bool WSDAMCache::sort_helper(WSDAMDirectorySet i, WSDAMDirectorySet j) {
  return i.access_count > j.access_count;
}
