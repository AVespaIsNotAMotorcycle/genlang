/*
word_hashing.hpp
Sam Ebersole
Defines a hash table using double hashing on Words. 
Modified from an assignment for CSCI33500.
November 6, 2020
*/

#ifndef WORD_HASHING_H
#define WORD_HASHING_H

#include <vector>
#include <algorithm>
#include <functional>
#include "word.hpp"

namespace {

/*
  bool IsPrime(int n) {
    int i;
    for(i=2; i<=n/2; ++i) {
        if(n%i==0) {
          return false;
        }
    }
    return true;
  }

  // Internal method to return a prime number at least as large as n.
  int NextPrime(size_t n) {
    if (n % 2 == 0)
      ++n;
    while (!IsPrime(n)) n += 2;
    return n;
  }
*/
} // namespace

// Double hashing probing implementation.
class HashTableWord {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableWord(size_t size = 101) : array_(NextPrime(size))
    { MakeEmpty(); }

  bool Contains(const Word & x) const {
    return IsActive(FindPos(x));
  }

  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const Word & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;

    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();
    return true;
  }

  bool Insert(Word && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;

    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const Word & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

  // MUTATOR

  bool SetR(int r) {
    r_value_ = r;
  }

  // ACCESSORS

  int GetSize() {
    return array_.size();
  }

  int GetItemCount() {
    return current_size_;
  }

  int GetCollisions() {
    return collisions_;
  }

    void PrintAll() {
        EntryType t = ACTIVE;
        for (int i = 0; i < array_.size(); i++) {
            if (array_[i].info_ == t) {
                array_[i].element_.print();
            }
        }
    }

  // returns the number of probes needed to find an entry
  // number returned is negative if not found
  Word GetEntry(const Word & x) {
    int probes = 1;
    size_t current_pos = InternalHash(x);
    size_t double_hash = DoubleHash(x);

    //std::cout << "-----------------------" << std::endl << "SEARCHING FOR " << x  << ", HASH " << InternalHash(x) << std::endl;

    //std::cout << array_[current_pos].element_ << ", HASH " << InternalHash(array_[current_pos].element_) << ", PROBE COUNT " << probes << std::endl;
    while (array_[current_pos].info_ != EMPTY && array_[current_pos].element_ != x) {
      probes++;
      current_pos += double_hash;  // Compute ith probe.
      if (current_pos >= array_.size())
         current_pos -= array_.size();

     //std::cout << array_[current_pos].element_ << ", HASH " << InternalHash(array_[current_pos].element_) << ", PROBE COUNT " << probes << std::endl;
    }
    
    if (array_[current_pos].info_ == EMPTY) {
      return x;
    }
    else {
      return (array_[current_pos].element_);
    }
  }

 private:
  struct HashEntry {
    Word element_;
    EntryType info_;

    HashEntry(const Word& e = Word(), EntryType i = EMPTY)
    :element_{e}, info_{i} { }

    HashEntry(Word && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };


  std::vector<HashEntry> array_;
  size_t current_size_;

  int collisions_ = 0;
  int r_value_ = 127;

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  size_t FindPos(const Word & x) const {
    size_t current_pos = InternalHash(x);
    size_t double_hash = DoubleHash(x);

    while (array_[current_pos].info_ != EMPTY && array_[current_pos].element_ != x) {
      //collisions_++;
      current_pos += double_hash;  // Compute ith probe.
      if (current_pos >= array_.size())
	       current_pos -= array_.size();
    }
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;

    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }

  size_t InternalHash(const Word & x) const {
    static std::hash<string> hf;
    return hf(x.getDefCode()) % array_.size( );
  }

  size_t DoubleHash(const Word & x) const {
    static std::hash<string> hf;
    return r_value_ - (hf(x.getDefCode()) % r_value_);
  }

};

#endif  // DOUBLE_HASHING_H
