// AVLSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"


template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;

  
  
private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
  struct TreeNode
  {
    ElementType key;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    int height = 0;

    TreeNode(const ElementType& info) : key(info), left(nullptr),  right(nullptr), height(0) {}
  };
  TreeNode* root;
  int sz;
  bool bal;
  void copyOldTree(TreeNode* curr);
  TreeNode* insertIt(TreeNode* curr, ElementType key);
  void helpPre(VisitFunction visit, TreeNode* curr) const;
  void helpIn(VisitFunction visit, TreeNode* curr) const;
  void helpPos(VisitFunction visit, TreeNode* curr) const;
  int findH(TreeNode* curr);
  int balH(TreeNode* curr);
  int setH(TreeNode* curr);
  TreeNode* RR(TreeNode* &curr);
  TreeNode* RL(TreeNode* &curr);
  void delChild(TreeNode* curr);
};



template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
  bal = shouldBalance; // otherwise it will become a Binary Search Tree
  root = nullptr;
  sz = 0;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
  delChild(root);
}

template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
  root = nullptr;
  sz = s.sz;
  copyOldTree(s.root);
}

template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
  root = nullptr;
  sz = s.sz;
  if(s.root)
    {
      copyOldTree(s.root);
    }
}

template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this != &s)
    {
      this->sz = s.sz;
      this->delChild(root);
      root = nullptr;
      this->copyOldTree(s.root);
    }
  return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
  if(this != &s)
    {
      this->sz = s.sz;
      this->delChild(root);
      root = nullptr;
      this->copyOldTree(s.root);
    }
  return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
  return true;
}

template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
  root = insertIt(root, element);
  sz++;
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
  TreeNode* curr = root;
  while(curr)
    {
      if(curr->key == element)
        {
          return true;
        }
      else if(element > curr->key)
        {
          curr = curr->right;
        }
      else
        {
          curr = curr->left;
        }
    }
  return false;
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
  return !root? 0: sz; 
}

template <typename ElementType>
int AVLSet<ElementType>::height() const
{
  return !root? -1: root->height;
}

template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
  helpPre(visit, root);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
  helpIn(visit, root);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
  helpPos(visit, root);
}

//******HELPER FUNCTIONS GO HERE******//

template <typename ElementType>
void AVLSet<ElementType>::copyOldTree(TreeNode* curr)
{
  if(curr)
    {
      add(curr->key);
      copyOldTree(curr->right);
      copyOldTree(curr->left);
    }
}

template <typename ElementType>
typename AVLSet<ElementType>::TreeNode* AVLSet<ElementType>::insertIt(TreeNode* curr, ElementType key)
{
  if (!curr)
    {
      TreeNode* temp = new TreeNode(key);
      return temp;
    }
  key > curr->key? curr->right = insertIt(curr->right, key): curr->left = insertIt(curr->left, key);
  
  if(bal) // if AVL tree -> will need to be balanced otherwise BST
    {
      curr->height = setH(curr);
      int numB = balH(curr);
      if(numB > 1)
        {
          if(curr->left->key > key)
            {
              return RR(curr);
            }
          if(curr->left->key < key)
            {
              curr->left = RL(curr->left);
              return RR(curr);
            }
        }
      if(numB < -1)
        {
          if(curr->right->key < key)
            {
              return RL(curr);
            }
          if(curr->right->key > key)
            {
              curr->right = RR(curr->right);
              return RL(curr);
            }
        }
    }
  else
    {
      curr->height = setH(curr);
    }
  return curr;
}

template <typename ElementType>
void AVLSet<ElementType>::helpPre(VisitFunction visit, TreeNode* curr)const
{
  if(!curr)
    {
      return;
    }
  visit(curr->key);
  helpPre(visit, curr->left);
  helpPre(visit, curr->right);
}

template <typename ElementType>
void AVLSet<ElementType>::helpIn(VisitFunction visit, TreeNode* curr)const
{
   if(!curr)
    {
      return;
    }
   helpIn(visit, curr->left);
   visit(curr->key);
   helpIn(visit, curr->right);
}

template <typename ElementType>
void AVLSet<ElementType>::helpPos(VisitFunction visit, TreeNode* curr)const
{
   if(!curr)
    {
      return;
    }
   helpPos(visit, curr->left);
   helpPos(visit, curr->right);
   visit(curr->key);
}

template <typename ElementType>
int AVLSet<ElementType>::findH(TreeNode* curr)
{
  return !curr? -1: curr->height;
}

template <typename ElementType>
int AVLSet<ElementType>::balH(TreeNode* curr)
{
  return !curr? 0: findH(curr->left) - findH(curr->right);
}

template <typename ElementType>
int AVLSet<ElementType>::setH(TreeNode* curr)
{
  return std::max(findH(curr->right), findH(curr->left))+1;
}
  
template <typename ElementType>
typename AVLSet<ElementType>::TreeNode* AVLSet<ElementType>::RR(TreeNode* &curr)
{
  TreeNode* tp1;
  TreeNode* tp2;
  tp1 = curr->left;
  tp2 = tp1->right;
  tp1->right = curr;
  curr->left = tp2;
  
  curr->height = setH(curr);
  tp1->height = setH(tp1);
  return tp1;
}

template <typename ElementType>
typename AVLSet<ElementType>::TreeNode* AVLSet<ElementType>::RL(TreeNode* &curr)
{
  TreeNode* tp1;
  TreeNode* tp2;
  tp1 = curr->right;
  tp2 = tp1->left;
  tp1->left = curr;
  curr->right = tp2;
  
  curr->height = setH(curr);
  tp1->height = setH(tp1);
  return tp1;
}

template <typename ElementType>
void AVLSet<ElementType>::delChild(TreeNode* curr)
{
  if(curr)
    {
      if(curr->right)
        {
          delChild(curr->right);
        }
      if(curr->left)
        {
          delChild(curr->left);
        }
      delete curr;   
    }
}

#endif // AVLSET_HPP

