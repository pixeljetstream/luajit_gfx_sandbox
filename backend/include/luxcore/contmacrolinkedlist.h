// Copyright (C) 2004-2011 Christoph Kubisch & Eike Decker
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#ifndef __LUXCORE_MACROLINKEDLIST_H__
#define __LUXCORE_MACROLINKEDLIST_H__

//////////////////////////////////////////////////////////////////////////
// Macro-based Double Linked List
//
// Each struct must contain 2 pointers (next & prev) to own type
// The LN_ - macros allows manual naming of next & prev variables
// The lxListNode_ macros assume next & prev as variable names
// Don't use this in C++, prefer templates


#ifndef NULL
  #define NULL 0
#endif

#ifndef XOrSwap
  #define XOrSwap(a,b) {*(int*)&(a)^=*(int*)&(b);*(int*)&(b)^=*(int*)&(a);*(int*)&(a)^=*(int*)&(b);}
#endif


////////////////////////////////////////////////////////////////////////////////
// LN_
// Generalized linked list node system

// initialises the node - next = prev = this
#define lxLN_init(node,next,prev) {node->next=node; node->prev=node;}

#define lxLN_next(node,next,prev) (node->next)
#define lxLN_prev(node,next,prev) (node->prev)

// inserts a node next to the prevNode
#define lxLN_insertNext(prevNode,insertNode,next,prev) {\
  if (prevNode!=NULL) { \
    prevNode->next->prev = insertNode;\
    prevNode->next->prev->next = prevNode->next;\
    prevNode->next->prev->prev = prevNode;\
    prevNode->next = prevNode->next->prev;\
  } else prevNode = insertNode;\
}
// inserts a node before nextNode
#define lxLN_insertPrev(nextNode,insertNode,next,prev) {\
  if (nextNode!=NULL) { \
    nextNode->prev->next = insertNode;\
    nextNode->prev->next->prev = nextNode->prev;\
    nextNode->prev = nextNode->prev->next;\
    nextNode->prev->next = nextNode;\
  } else nextNode = insertNode;\
}

// appends listB to the end of listA
#define lxLN_append(listA,listB,next,prev) {\
  XOrSwap(listA->prev,listB->prev); \
  listA->prev->next = listA;\
  listB->prev->next = listB;}

// we will try to set browse to the node at given index
// if we cant find it, we will set browse = NULL
// and indexCounter will have the amount of nodes to be 
// added to make the Index happen
#define lxLN_getAtIndex(listNodeHead, browse, indexCounter,next,prev)\
  lxLN_getAtOffset(listNodeHead,listNodeHead, browse, indexCounter,next,prev)
#define lxLN_getAtOffset(listNodeHead,base, browse, indexCounter,next,prev) { \
  browse = base; \
  if (indexCounter && browse){\
    if (indexCounter>0){\
      while (indexCounter > 0){\
        if (browse->next == listNodeHead)\
          break;\
        browse = browse->next;\
        indexCounter--;\
      }\
    }\
    else if (indexCounter<0){\
      while (indexCounter < 0){\
        if (browse->prev == listNodeHead->prev)\
          break;\
        browse = browse->prev;\
        indexCounter++;\
      }\
    }\
    /* we havent found anything */\
    if (indexCounter != 0)\
      browse = NULL;\
  }\
  if (!listNodeHead)\
    indexCounter++;\
}
// stores position within the list in "pos"
#define lxLN_getNodeIndex(listNodeHead, browse, pos,next,prev) { \
  pos = 0;\
  browse = listNodeHead; \
  do { \
    pos++; browse = browse->next; \
  } while (listNodeHead!=browse); \
}

// removes a node - the head could be any listnodepointer
#define lxLN_remove(listNodeHead,removeNode,next,prev) {\
  if (listNodeHead && removeNode){\
  if (listNodeHead == removeNode || listNodeHead==NULL) listNodeHead = removeNode->next;\
  removeNode->prev->next = removeNode->next;\
  removeNode->next->prev = removeNode->prev;\
  removeNode->next = removeNode->prev = removeNode; \
  if (listNodeHead == removeNode) listNodeHead = NULL; \
  }\
}

// remove a node from a list without head - results in a singleitem list
#define lxLN_rip(listnode,next,prev) {\
  if (listnode->next!=listnode) {\
    listnode->prev->next = listnode->next;\
    listnode->next->prev = listnode->prev;\
    listnode->next = listnode->prev = listnode;\
  }\
}

// counts size of nodes, 
// listNode: node of the listn; 
// browse: valid browsing variable (will be used)
// iSize: integer - result of counting
#define lxLN_countSize(listNode,browse,iSize,next,prev) {\
  iSize = 0;\
  browse = listNode;\
  if (browse != NULL)\
  do {\
    browse = browse->next;\
    iSize++;\
  } while (browse != listNode);\
}

// forEachConinue for continuing
#define lxLN_forEachContinue(listNode,browse,next,prev) \
  {browse = browse->next; \
  if (browse == listNode) break; else continue;}

// lxListNode_forEach using next
#define lxLN_forEach(listNode,browse,next,prev) {\
  if (listNode != NULL){ \
  browse = (listNode);\
  do { 
#define lxLN_forEachEnd(listNode,browse,next,prev) \
    browse = browse->next;\
  } while (browse!=(listNode) && listNode!=NULL);\
  } \
}

// add last
#define lxLN_addLast(listNodeHead,node,next,prev) {\
  if (listNodeHead){\
    lxLN_insertPrev(listNodeHead,node,next,prev);\
  }else\
    listNodeHead = node;\
}
// add first
#define lxLN_addFirst(listNodeHead,node,next,prev)  {\
  lxLN_insertPrev(listNodeHead,node,next,prev); \
  listNodeHead = node; \
}

// Push / Pop
// popps the first node of the list 
// thePoppedNode: any valid pointer for the node
#define lxLN_pushFront(listNodeHead, pushedNode,next,prev) \
    lxLN_addFirst(listNodeHead, pushedNode,next,prev)
#define lxLN_popFront(listNodeHead, thePopedNode,next,prev) {\
  thePopedNode = listNodeHead;\
  lxLN_remove(listNodeHead, thePopedNode,next,prev); \
}

#define lxLN_pushBack(listNodeHead, pushedNode,next,prev) lxLN_addLast(listNodeHead,pushedNode,next,prev)
#define lxLN_popBack(listNodeHead, thePopedNode,next,prev)  {\
  if (listNodeHead){  \
    thePopedNode = listNodeHead->prev;  \
    lxLN_remove(listNodeHead,thePopedNode,next,prev); \
  }\
  else thePopedNode = NULL; \
}


// swaps nodes that are next to each other
#define lxLN_swapNeighbor(listNodeHead,a, b,next,prev) {\
  if ( a && b && listNodeHead){\
  if (a->next == b){\
    lxLN_remove(listNodeHead,a,next,prev);\
    lxLN_insertNext(b,a,next,prev);\
  }\
  else{\
    lxLN_remove(listNodeHead,b,next,prev);\
    lxLN_insertNext(a,b,next,prev);\
  }\
  }\
}

#define lxLN_swapContent(Type,a,b,next,prev) {\
  Type d_temp;  \
  memcpy(&d_temp, a, sizeof(Type));\
  memcpy(a,b,sizeof(Type));\
  memcpy(b,&d_temp,sizeof(Type));\
  b->next = a->next;\
  a->next = d_temp.next;\
  b->prev = a->prev;\
  a->prev = d_temp.prev;\
}

#define lxLN_destroyList(listNodeHead, Type, destroyFunc,next,prev) {\
  while (listNodeHead != NULL)              \
  {                           \
    Type *d_node;                   \
    lxLN_popFront(listNodeHead, d_node,next,prev);      \
    destroyFunc(d_node);                  \
  }                           \
}

// sorts the nodes using cmpTest func
// if cmpTest returns a value a swap is done
#define lxLN_sortBubble(listNodeHead,Type,cmpFunc,next,prev) {\
  if (listNodeHead){\
    uchar d_change = 1;   \
    int d_count,d_i;    \
    Type *d_browse;       \
    lxLN_countSize(listNodeHead,d_browse,d_count,next,prev);  \
    d_count--;          \
    while(d_change){      \
      d_change = 0;     \
      d_i = 0;        \
      lxLN_forEach(listNodeHead,d_browse,next,prev)         \
        if (d_i < d_count){             \
          if (cmpFunc(d_browse,d_browse->next)){  \
            lxLN_swapContent(Type,d_browse,d_browse->next,next,prev);\
            d_change = 1;\
          }\
        }\
        else break;\
        d_i++;\
      lxLN_forEachEnd(listNodeHead,d_browse,next,prev);\
      d_count--;\
    }\
  }\
}

// From http://efgh.com/software/llmsort.htm
// Author: Philip J. Erdelsky
#define lxLN_sortTapeMerge(listNodeHead,Type,cmpFunc,next,prev)   \
{                                 \
  uint d_base;                            \
  uint d_block_size;                        \
  Type  *d_browse;                          \
                                  \
  struct tape_s                           \
  {                                 \
    Type *first, *last;                       \
    uint count;                           \
  } d_tape[4];                            \
                                  \
  /* Distribute the records alternately to tape[0] and tape[1]. */  \
  memset(d_tape,0,sizeof(struct tape_s)*4);             \
                                  \
  d_tape[0].count = d_tape[1].count = 0;              \
  d_tape[0].first = NULL;                     \
  d_base = 0;                           \
  while (listNodeHead != NULL)                    \
  {                                 \
    lxLN_popFront(listNodeHead,d_browse,next,prev);         \
    lxLN_pushBack(d_tape[d_base].first,d_browse,next,prev);     \
    d_tape[d_base].count++;                     \
    d_base ^= 1;                          \
  }                                 \
                                  \
  /* If the list is empty or contains only a single record, then */ \
  /* tape[1].count == 0L and this part is vacuous.               */ \
                                  \
  for (d_base = 0, d_block_size = 1; d_tape[d_base+1].count != 0; \
    d_base ^= 2, d_block_size <<= 1)                \
  {                                 \
    int d_dest;                           \
    struct tape_s *d_tape0, *d_tape1;               \
    d_tape0 = d_tape + d_base;                    \
    d_tape1 = d_tape + d_base + 1;                  \
    d_dest = d_base ^ 2;                      \
    d_tape[d_dest].count = d_tape[d_dest+1].count = 0;        \
    for (; d_tape0->count != 0; d_dest ^= 1)            \
    {                               \
      uint n0, n1;                          \
      struct tape_s *d_output_tape = d_tape + d_dest;       \
      n0 = n1 = d_block_size;                   \
      while (1)                           \
      {                               \
        Type *d_chosen_record;                    \
        struct tape_s *d_chosen_tape;               \
        if (n0 == 0 || d_tape0->count == 0)             \
        {                             \
          if (n1 == 0 || d_tape1->count == 0)           \
            break;                          \
          d_chosen_tape = d_tape1;                  \
          n1--;                           \
        }                             \
        else if (n1 == 0 || d_tape1->count == 0)          \
        {                             \
          d_chosen_tape = d_tape0;                  \
          n0--;                           \
        }                             \
    else if (cmpFunc(d_tape0->first,d_tape1->first))      \
        {                             \
          d_chosen_tape = d_tape1;                  \
          n1--;                           \
        }                             \
        else                            \
        {                             \
          d_chosen_tape = d_tape0;                  \
          n0--;                           \
        }                             \
    d_chosen_tape->count--;                   \
    lxLN_popFront(d_chosen_tape->first,d_chosen_record,next,prev);\
    if (d_output_tape->count == 0)                \
      d_output_tape->first = NULL;              \
        lxLN_pushBack(d_output_tape->first,d_chosen_record,next,prev);\
        d_output_tape->count++;                   \
      }                               \
    }                               \
  }                                 \
                                  \
  listNodeHead = d_tape[d_base].first;                \
}

//////////////////////////////////////////////////////////////////////////
// lxListNode
// next,prev as names predefinition

#define LUX_LISTNODEVARS *prev, *next

// initialises the node - next = prev = this
#define lxListNode_init(node) lxLN_init(node,next,prev)

#define lxListNode_next(node) lxLN_next(node,next,prev)
#define lxListNode_prev(node) lxLN_prev(node,next,prev)

// inserts a node next to the prevNode
#define lxListNode_insertNext(prevNode,insertNode)  lxLN_insertNext(prevNode,insertNode,next,prev)
// inserts a node before nextNode
#define lxListNode_insertPrev(nextNode,insertNode)  lxLN_insertPrev(nextNode,insertNode,next,prev)

// appends listB to the end of listA
#define lxListNode_append(listA,listB)        lxLN_append(listA,listB,next,prev)

// we will try to set browse to the node at given index
// if we cant find it, we will set browse = NULL
// and indexCounter will have the amount of nodes to be 
// added to make the Index happen
#define lxListNode_getAtIndex(listNodeHead, browse, indexCounter)\
  lxListNode_getAtOffset(listNodeHead,listNodeHead, browse, indexCounter)
#define lxListNode_getAtOffset(listNodeHead,base, browse, indexCounter) lxLN_getAtOffset(listNodeHead,base, browse, indexCounter,next,prev)
// stores position within the list in "pos"
#define lxListNode_getNodeIndex(listNodeHead, browse, pos)        lxLN_getNodeIndex(listNodeHead, browse, pos,next,prev)

// removes a node - the head could be any listnodepointer
#define lxListNode_remove(listNodeHead,removeNode)            lxLN_remove(listNodeHead,removeNode,next,prev)

// remove a node from a list without head - results in a singleitem list
#define lxListNode_rip(listnode)    lxLN_rip(listnode,next,prev)

// counts size of nodes, 
// listNode: node of the list; 
// browse: valid browsing variable (will be used)
// iSize: integer - result of counting
#define lxListNode_countSize(listNode,browse,iSize) lxLN_countSize(listNode,browse,iSize,next,prev)     

// forEachConinue for continuing
#define lxListNode_forEachContinue(listNode,browse) lxLN_forEachContinue(listNode,browse,next,prev)

// lxListNode_forEach iteration using next
#define lxListNode_forEach(listNode,browse)     lxLN_forEach(listNode,browse,next,prev)
#define lxListNode_forEachEnd(listNode,browse)    lxLN_forEachEnd(listNode,browse,next,prev)

// add last, in case node is the first, it must be inited
#define lxListNode_addLast(listNodeHead,node)     lxLN_addLast(listNodeHead,node,next,prev)
// add first
#define lxListNode_addFirst(listNodeHead,node)    lxLN_addFirst(listNodeHead,node,next,prev)

// Push / Pop
// popps the first node of the list 
// thePoppedNode: any valid pointer for the node
#define lxListNode_pushFront(listNodeHead, pushedNode)  lxLN_addFirst(listNodeHead, pushedNode,next,prev)
#define lxListNode_popFront(listNodeHead, thePoppedNode)  lxLN_popFront(listNodeHead, thePoppedNode, next,prev)

#define lxListNode_pushBack(listNodeHead, pushedNode)   lxLN_addLast(listNodeHead,pushedNode,next,prev)
#define lxListNode_popBack(listNodeHead, thePoppedNode) lxLN_popBack(listNodeHead, thePoppedNode, next,prev)


// swaps nodes that are next to each other
#define lxListNode_swapNeighbor(listNodeHead,a, b)    lxLN_swapNeighbor(listNodeHead,a, b, next,prev)
#define lxListNode_swapContent(Type,a,b)          lxLN_swapContent(Type,a,b,next,prev)

#define lxListNode_destroyList(listNodeHead, Type, destroyFunc) lxLN_destroyList(listNodeHead, Type, destroyFunc, next,prev)


// sorts the nodes using cmpTest func
// if cmpTest returns a value a swap is done
#define lxListNode_sortBubble(listNodeHead,Type,cmpFunc)    lxLN_sortBubble(listNodeHead,Type,cmpFunc,next,prev)
#define lxListNode_sortTapeMerge(listNodeHead,Type,cmpFunc) lxLN_sortTapeMerge(listNodeHead,Type,cmpFunc, next,prev)



#endif
