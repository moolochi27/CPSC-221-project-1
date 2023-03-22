#include "chain.h"
#include <cmath>
#include <iostream>
#include "block.h"
#include <limits.h>
using namespace std;


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  clear();
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {
  length_++;
  Node * added = new Node(ndata);
  if(p == NULL){
    added->prev = NULL;
    added->next = head_;
    if (head_ != NULL)
        head_->prev = added;
    head_ = added;
  }
  else{
    Node * temp = p->next;
    p->next = added;
    if(temp != NULL){temp->prev = added;}

    added->next = temp;
    added->prev = p;
  }
  return added;
}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  if(p == NULL || q == NULL || p == q){return;}

  Node * temp1 = p->next;
  Node * temp2 = p->prev;
  Node * temp3 = q->next;
  Node * temp4 = q->prev;

//if p is right before q
  if(p->next == q){
    q->next = p;
    q->prev = temp2;
    p->next = temp3;
    p->prev = q;
  }
  //if q is right before p
  else if(q->next == p){
    q->next = temp1;
    q->prev = p;
    p->next = q;
    p->prev = temp4;
  }
  //the other cases.
  else{
    p->next = temp3;
    p->prev = temp4;

    q->next= temp1;
    q->prev = temp2;
  }
  //fixing the things that point to p and q, as well as updating head_ if necessary.
  if (p->prev != NULL)
      p->prev->next = p;
  else{head_ = p;}

  if (p->next != NULL)
      p->next->prev = p;
  
  if (q->prev != NULL)
      q->prev->next = q;
  else{head_ = q;}

  if (q->next != NULL)
      q->next->prev = q;
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  while(head_ != NULL){
    Node * temp = head_;
    head_ = head_->next;
    delete temp;
  }
  length_ = 0;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
  clear();
  Node* c = other.head_;
  insertAfter(NULL, c->data);
  c = c->next;
  Node * prevBlock = head_;

  while (c != NULL){
    insertAfter(prevBlock, c->data);
    prevBlock = prevBlock->next;
    c = c->next;
  }
}

/* Modifies the current chain:
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::unscramble() {
  //finding the start
  Node * start = findStarter();
  //swaping the start with the head.
  swap(head_, start);

  //swaping algorithm for the rest:
  Node *p = start;
  while(p != NULL){
    Node * toSwap = findNext(p);
    //swapping the nexter block with the block after p.
    swap(p->next, toSwap);
    //changing p.
    p = p->next;
  }
}

  //Helper functions for unscramble


  /**
  *Finds the head block of the unscrambled image.
  */
  Chain::Node * Chain::findStarter() {
    Node * p = head_;
    Node * start = head_;
    double block_distance = 0;

    for (int i = 0; i < length_; i++){
      Node * prevBlock = p->prev;
      Node * nextBlock = p->next;
      double distance = INT_MAX;

      while(prevBlock != NULL){
        distance = min(distance, prevBlock->data.distanceTo(p->data));
        prevBlock = prevBlock->prev;
      }

      while(nextBlock != NULL){
        distance = min(distance, nextBlock->data.distanceTo(p->data));
        nextBlock = nextBlock->next;
      }

      if(block_distance < distance){
        block_distance = distance;
        start = p;
      }
      p = p->next;
    }
    return start;
  }

  /**
  * Finds the block that should be next to position p in the unscrambled image.
  */
  Chain::Node * Chain::findNext(Node * current){
    Node * toswap = NULL;
    double distance = INT_MAX;
    Node * nextBlock = current->next;

    //Finding the next block nexter.
    while(nextBlock != NULL){
      if(distance > current->data.distanceTo(nextBlock->data)){
        distance = current->data.distanceTo(nextBlock->data);
        toswap = nextBlock;
      }
      nextBlock = nextBlock->next;
    }

    return toswap;
  }


