// HashSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;
     struct ListNode
     {
       ElementType key;
       ListNode* next = nullptr;       
     };
  ListNode** head;
  int cap = 0;
  int sz = 0;
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
  cap = DEFAULT_CAPACITY;
  sz = 0;
  head = new ListNode*[cap];
  for(int i = 0; i < cap; ++i)
    {
      head[i] = nullptr;
    }
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
  ListNode* temp;
  for(int i = 0; i < cap; ++i)
    {
      temp = head[i];
      while(temp)
        {
          ListNode* prev = temp;
          temp = temp->next;
          delete prev;
        }
    }
  delete[] head;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
  cap = s.cap;
  hashFunction = s.hashFunction;
  sz = s.sz;
  head = new ListNode*[cap];
  for (int i = 0; i < cap; ++i)
    {
      ListNode* start = s.head[i];
      ListNode* temp = nullptr;
      while (start)
        {
          temp = new ListNode{start->key, temp};
          start = start->next;
        }
      head[i] = temp;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
  cap = DEFAULT_CAPACITY;
  sz = 0;
  head = new ListNode*[cap];
  for(int i = 0; i < cap; ++i)
    {
      head[i] = nullptr;
    }
  std::swap(hashFunction, s.hashFunction);
  std::swap(sz, s.sz);
  std::swap(cap, s.cap);
  std::swap(head, s.head);
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
   if(this != &s)
     {
       for(int i = 0; i < cap; ++i)
         {
           ListNode* temp = head[i];
           while(temp)
             {
               ListNode* prev = temp;
               temp = temp->next;
               delete prev;
             }
         }
    
       delete[] head;
    
       cap = s.cap;
       sz = s.sz;
       hashFunction = s.hashFunction;
       head = new ListNode*[cap];
       for (int i = 0; i < cap; ++i)
         {
           ListNode* start = s.head[i];
           ListNode* temp = nullptr;
           while(start)
             {
               temp = new ListNode{start->key, temp};
               start = start->next;
             }
           head[i] = temp;
         }
    }
  return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
   if(this != &s)
     {
       std::swap(head, s.head);
       std::swap(cap, s.cap);
       std::swap(sz, s.sz);
       std::swap(hashFunction, s.hashFunction);
     }
  return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
  int i = hashFunction(element) % cap;
  ListNode* item = head[i];
  while (item)
    {
      if (item->key == element)
        return;
      item = item->next;
    }
  
  head[i] = new ListNode{element, head[i]};
  sz++;
  if(0.8*cap < sz) // resize if ratios over 0.8
    {
      ListNode** temp = new ListNode* [cap];
      for (int j = 0; j < cap; ++j)
        {
          ListNode* curr = head[j];
          ListNode* a = nullptr;
          while (curr)
            {
              a = new ListNode{curr->key, a};
              curr = curr->next;
            }
          temp[j] = a;
        }
      ListNode* b;
      for(int k  = 0; k < cap; ++k)
        {
          b = head[k];
          while(b)
            {
              ListNode* prev = b;
              b = b->next;
              delete prev;
            }
        }
      delete [] head;
      
      int old_cap = cap;
      cap *=2;
      head = new ListNode* [cap];
      for(int p = 0;  p < cap; ++p)
        {
          head[p] = nullptr;
        }
      for(int q = 0; q < old_cap; ++q)
        {
          ListNode* temp1 = temp[q];
          while(temp1)
            {
              ElementType k = temp1->key;
              int index = hashFunction(k) % cap;
              head[index] = new ListNode{k, head[index]};
              temp1 = temp1->next;
            }
        }
      
    }
 
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
  ListNode* curr = head[hashFunction(element) % cap];
  while(curr)
    {
      if(curr->key == element)
        {
          return true;
        }
      else
        {
          curr = curr->next;
        }
    }
  return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
  return static_cast<unsigned int> (sz);
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
  if(index < 0 || index >= cap)
    {
      return 0;
    }
  else
    {
      ListNode* curr = head[index];
      int count = 0;
      while(curr)
        {
          count++;
          curr = curr->next;
        }
      return count;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
  if (index < 0 || index >= cap)
    {
      return false;
    }
  ListNode* curr = head[index];
  while(curr)
  {
    if(curr->key == element)
      {
        return true;
      }
    curr = curr->next;
  }
  return false;
}



#endif // HASHSET_HPP

