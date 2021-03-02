#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  clear();
  head_ = NULL;
  length_ = 0;

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
  Node *toInsert = new Node(ndata);
  if (p == NULL) {
  	if (head_ == NULL) {
	  	head_ = toInsert;
  	} else {
  		toInsert->next = head_;
	  	toInsert->prev = NULL;
	  	head_ = toInsert;
  	}
  } else if (p->next == NULL) {
  	toInsert->next = NULL;
  	toInsert->prev = p;
  	p->next = toInsert;
  } else {
  	Node *n = p->next;
  	p->next = toInsert;
	  n->prev = toInsert;
	  toInsert->next = n;
	  toInsert->prev = p;
  }
  length_++;
  return toInsert;
}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  if (p == NULL || q == NULL) {
  	return;
  } else if (p==q) {
  	return;
  } else {
  	Node *p_prev = p->prev;
    Node *p_next = p->next;
    Node *q_prev = q->prev;
    Node *q_next = q->next;

    if (p->next == q) {
      if (p != head_) {
        p_prev->next = q;
      }
      if (q->next != NULL) {
        q_next->prev = p;
      }
      q->prev = p_prev;
      q->next = p;
      p->prev = q;
      p->next = q_next;
    } else if (q->next == p) {
      if (q != head_) {
        q_prev->next = p;
      }
      if (p->next != NULL) {
        p_next->prev = q;
      }
      q->prev = p;
      q->next = p_next;
      p->prev = q_prev;
      p->next = q;
    } else {
      if (p != head_) {
        p_prev->next = q;
      } 
      if (q != head_) {
        q_prev->next = p;
      }
      if (p->next != NULL) {
        p_next->prev = q;
      }
      
      if (q->next != NULL) {
        q_next->prev = p;
      }
      q->prev = p_prev;
      q->next = p_next;
      p->prev = q_prev;
      p->next = q_next;
    }
    if (q == head_) {
      head_ = p;
    } else if (p == head_) {
      head_ = q;
    }
  }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  Node *curr = head_;
  Node *temp = head_;
  while (curr != NULL) {
    curr = curr->next;
    delete temp;
    temp = curr;
  }
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
  Node *other_curr = other.head_;
  Node *curr = new Node(other_curr->data);
  head_ = curr;

  other_curr = other_curr->next;
  while(other_curr != NULL) {
    Node *toInsert = new Node(other_curr->data);
    curr->next = toInsert;
    toInsert->prev = curr;
    curr = curr->next;
    other_curr = other_curr->next;
  }
  //curr->next = head_;
  //head_->prev = curr;
  length_ = other.length_;

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
  double max = __DBL_MIN__;
  double min = __DBL_MAX__;
  Node *curr;
  Node *maxNode;
  Node *minNode;

  for (curr = head_; curr != NULL; curr = curr->next) { 
    for (Node *curr2 = head_; curr2 != NULL; curr2 = curr2->next) {
      double distance = curr2->data.distanceTo(curr->data);
      if (distance < min && min != 0) {
        min = distance;
      }
    }
    if (min > max) {
      max = min;
      maxNode = curr;
    }
    min = __DBL_MAX__;
  }

  swap(head_, maxNode);

  for (curr = head_; curr != NULL; curr=curr->next) {
    for (Node *curr2 = curr->next; curr2 != NULL; curr2=curr2->next) {
      double distance = curr->data.distanceTo(curr2->data);
      if (distance < min) {
        min = distance;
        minNode = curr2;
      }
    }
    if (curr->next != NULL) {
      swap(minNode, curr->next);
    }
    min = __DBL_MAX__;
  }
} 
