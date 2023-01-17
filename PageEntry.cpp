#include "PageEntry.h"

//Constructors
PageEntry::PageEntry() {}

PageEntry::PageEntry(int fr_num, bool valid_bit){
    this->frameNum = fr_num;
    this->valid = valid_bit;
}

//Getters
int PageEntry::getFrameNum(){
  return this->frameNum;
}

bool PageEntry::getValidBit(){
  return this->valid;
}

//Setters
void PageEntry::setFrameNumber(int new_frame_number){
    this->frameNum = new_frame_number;
}

void PageEntry::setValidBit(bool new_valid_bit){
  this->valid = new_valid_bit;
}