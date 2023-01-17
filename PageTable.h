#pragma once

#include "PageEntry.h"
#include <vector>
#include <queue>

#define RANDOM 1
#define FIFO 2
#define LRU 3

using namespace std;

struct node {
  int page_number;
  node *next;
  node *back;
};

struct dllist{
  node *head;
  node *tail;
  int count;
  int max;

  //functs
  void remove();
  void insert(int);
};

/**
 * \brief A page table is like an array of page entries. The size of the page table should equal to the number of pages in logical memory
 */
class PageTable {
private:
  vector<PageEntry> pageTable;
  int numPages;
  unsigned int pageSize;
  int frameNumber{0};
  int maxFrames;
  queue<int> fifoQueue;
  dllist *lru;

public:
  PageTable(int, int);

  int newEntry(int, int);
  bool validPage(int);
  PageEntry& getEntry(int);

  //Page Replacement Algorithms
  void RANDOM_(int);
  void FIFO_(int);
  void LRU_(int);
};
