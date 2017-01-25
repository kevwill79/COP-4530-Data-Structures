// Kevin Williams
// COP 4530
// Proj5
// Section 2

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <algorithm>

namespace cop4530
{

static const unsigned int max_prime = 1301081;
static const unsigned int default_capacity = 11;
 
 template <typename K, typename V>
 class HashTable
  {
  public:
    HashTable(size_t size = 101);
    ~HashTable();
    
    bool contains(const K & k) const;
    bool match(const std::pair<K, V> & kv) const;
    bool insert(const std::pair<K, V> & kv);
    bool insert(std::pair<K, V> && kv);
    bool remove(const K & k);
    void clear();

    bool load(const char *filename);
    void dump() const;
    bool write_to_file(const char *filename) const;

    int getSize() const;

  private:
    size_t currentSize;
    std::vector <std::list<std::pair<K, V>>> bucketVector;
    
    void makeEmpty();
    void rehash();
    size_t myhash(const K &k) const;
    unsigned long prime_below(unsigned long);
    void setPrimes(std::vector<unsigned long> &);
  };

#include "hashtable.hpp"

}
#endif
