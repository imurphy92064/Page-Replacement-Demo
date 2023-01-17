#include "PageTable.h"
#include <iostream>
#include <random>
#include <cmath>

using namespace std;

//Constructors
PageTable::PageTable(int num_pages, int num_frames) {
  this->numPages = num_pages;
  this->maxFrames = num_frames;
  this->pageTable.reserve(num_pages);
  this->lru = new dllist {nullptr, nullptr, 0, num_frames};
}

int PageTable::newEntry(int page_number, int algo) {
  if (pageTable[page_number].getValidBit()){
    return 0;//This means the page table got a hit. No Page Fault.
  } 
  
  if (maxFrames == 0){//Execute a replacement algorithm
    this->maxFrames++;
    if (algo == RANDOM){//Random Algorithm
      RANDOM_(page_number);
    }
    else if(algo == FIFO){//FIFO Algorithm
      FIFO_(page_number);
    }
    else if(algo == LRU){//LRU Algorithm
      LRU_(page_number);
    }
    return 2;
  }

  if (algo == FIFO) {
    fifoQueue.push(page_number);
  }

  maxFrames--;
  pageTable[page_number].setFrameNumber(frameNumber++);
  pageTable[page_number].setValidBit(true);
  return 1;
}

//Checks to see if the page index is valid.
bool PageTable::validPage(int pageNum){
  if(pageNum > pageTable.capacity()){
    cout<<"The Page Index passed exceeds the Page Table size..."<<endl;
    return false;
  }
  else {
    return true;
  }
}

//Gets the Page Entry object in at the index passed.
PageEntry& PageTable::getEntry(int pageNumber) {
  return this->pageTable[pageNumber]; 
}

//Random page replacement algorithm when a page fault occurs.
void PageTable::RANDOM_(int page_number) {
  random_device source;
  unsigned int random_data[10];

  for(unsigned int& elem : random_data) {
        elem = source(); 
  }

  seed_seq seed(random_data + 0, random_data + 10);
  static default_random_engine generator(seed);

  static uniform_int_distribution<int> distribution(0,this->numPages - 1);
  int randomPageNumber = distribution(generator);
  pageTable[randomPageNumber].setValidBit(false);
  newEntry(page_number, 0);
}

//'First In First Out' page replacement algorithm when a page fault occurs.
void PageTable::FIFO_(int page_number){
  if(!fifoQueue.empty()){
    int page_index = fifoQueue.front();
    fifoQueue.pop();  
    if(validPage(page_index)){
      pageTable[page_index].setValidBit(false);  
    }
  }
  
  newEntry(page_number, 0);
}

//'Least Recently Used' page replacement algorithm when a page fault occurs.
void PageTable::LRU_(int page_number){
  this->lru->insert(page_number);

  newEntry(page_number, 0);
}

void dllist::insert(int page_number) {//Basically the LRU 
  //Seach for the page number
  node *start = this->tail;
  while (start != nullptr) {
    if (start->page_number == page_number) {
      break;
    }
    start = start->back;
  }

  if (start == nullptr && count == max) {
    remove();
  }

  if (start == nullptr) {//Didnt find the page number...Make 'start' the front of the link list
    node * newNode = new node {page_number, this->head, nullptr};

    if (this->count > 0) { // at least one page inside dllist
      this->head->back = newNode; // set current head back to the new Head
      this->head = newNode; // set head to the new head
    } else {
      this->head = this->tail = newNode; // nothing, so assign tail and head
    }
    
    this->count++;
  } else if (start != this->head) { //Found the page number...Make 'start' new head & shift the rest of the nodes
    this->head->back = start; // set current head back to the found node

    if (start == this->tail) { // found at the tail
      this->tail = start->back; // assign tail to the found node's back
      this->tail->next = nullptr; // remove tail's next
    } else { // somewhere that is not the head or tail
      start->back->next = start->next; // remove the found node from its current position
      start->next->back = start->back;
    }

    start->back = nullptr;
    start->next = this->head; // assign the new head to the found node
    this->head = start;
  }
}

void dllist::remove() {
  count--;
  node *newTail = this->tail->back;

  newTail->next = nullptr;
  this->tail = newTail;
}