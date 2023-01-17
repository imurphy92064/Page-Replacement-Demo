// Remember to add comments to your code

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "PageTable.h"

using namespace std;

// Check if an integer is power of 2
bool isPowerOfTwo(unsigned int x)
{
	/* First x in the below expression is for the case when x is 0 */
	return x && (!(x & (x - 1)));
}

int main(int argc, char* argv[]) {
	//Print basic information about the program
	cout << "=================================================================" << endl;
	cout << "CS 433 Programming assignment 5" << endl;
	cout << "Author: xxxxxx and xxxxxxx" << endl;
	cout << "Date: 05/03/2022" << endl;
	cout << "Course: CS433 (Operating Systems)" << endl;
	cout << "Description : Program to simulate different page replacement algorithms" << endl;
	cout << "=================================================================\n" << endl;

	if (argc < 3) {
		// user does not enter enough parameters
		cout << "You have entered too few parameters to run the program.  You must enter" << endl
			<< "two command-line arguments:" << endl
			<< " - page size (in bytes): between 256 and 8192, inclusive" << endl
			<< " - physical memory size (in megabytes): between 4 and 64, inclusive" << endl;
		exit(1);
	}
	
	// Page size and Physical memory size 
	// Their values should be read from command-line arguments, and always a power of 2
	unsigned int page_size = atoi(argv[1]);
	if(!isPowerOfTwo(page_size))
	{
		cout << "You have entered an invalid parameter for page size (bytes)" << endl
			<< "  (must be an power of 2 between 256 and 8192, inclusive)." << endl;
		return 1;
	}
	unsigned int phys_mem_size = atoi(argv[2]) << 20; // convert from MB to bytes
	if(!isPowerOfTwo(phys_mem_size))
	{
		cout << "You have entered an invalid parameter for physical memory size (MB)" << endl
			<< "  (must be an even integer between 4 and 64, inclusive)." << endl;
		return 1;
	}

	// calculate number of pages and frames;
	int logic_mem_bits = 27;		// 27-bit logical memory (128 MB logical memory assumed by the assignment)
	int phys_mem_bits = log2(phys_mem_size);		// Num of bits for physical memory addresses, calculated from physical memory size, e.g. 24 bits for 16 MB memory
	int page_offset_bits = log2(page_size);				// Num of bits for page offset, calculated from page size, e.g. 12 bits for 4096 byte page
	// Number of pages in logical memory = 2^(logic_mem_bits - page_bit)
	int num_pages = 1 << (logic_mem_bits - page_offset_bits);
	// Number of free frames in physical memory = 2^(phys_mem_bits - page_offset_bits)
	int num_frames = 1 << (phys_mem_bits - page_offset_bits);
	
	cout << "Page size = " << page_size << " bytes" << endl;
	cout << "Physical Memory size = " << phys_mem_size << " bytes" << endl;
	cout << "Number of pages = " << num_pages << endl; 
	cout << "Number of physical frames = " << num_frames << endl;

	// Test 1: Read and simulate the small list of logical addresses from the input file "small_refs.txt"
	cout <<"\n================================Test 1==================================================\n";
	// TODO: Add your code here for test 1 that prints out logical page #, frame # and whether page fault for each logical address
  PageTable test1PageTable(num_pages, num_frames);
  
  int numReferences{0};
  int numFaults{0};
  int numReplacements{0};

  ifstream small_s("small_refs.txt");
  string line;
  while (getline(small_s, line)) {
    int pageNumber = stoi(line) / page_size;

    if(test1PageTable.validPage(pageNumber)){
      int success = test1PageTable.newEntry(pageNumber, 0);

      numReferences++;
      if(success == 1){
        numFaults++;
      } else if (success == 2) {
        numReplacements++;
      }
  
      PageEntry entry = test1PageTable.getEntry(pageNumber);  
  
      cout << left << setw(32) << "Logical address: " + line + "," << setw(24) << "page number: " + to_string(pageNumber) + ", " << setw(24) << "frame number: " +                 
          to_string(entry.getFrameNum()) + "," << setw(24) << "is page fault? " + to_string(success) << endl;      
    }
    
  }
  cout<<"Number of References: " << numReferences << endl;
  cout<<"Number of Page Faults: " << numFaults << endl;
  cout<<"Number of Page Replacements: "<< numReplacements <<endl;
  
	// Test 2: Read and simulate the large list of logical addresses from the input file "large_refs.txt"
	cout <<"\n================================Test 2==================================================\n";
  clock_t timer1;
	cout << "****************Simulate FIFO replacement****************************" << endl;
	// TODO: Add your code to calculate number of page faults using FIFO replacement algorithm	
	// TODO: print the statistics and run-time
  timer1 = clock();
  PageTable test2PageTableFifo(num_pages, num_frames);
  
  int numReferencesFifo{0};
  int numFaultsFifo{0};
  int numReplacementsFifo{0};

  ifstream large_s_fifo("large_refs.txt");
  while (getline(large_s_fifo, line)) {
    int pageNumber = stoi(line) / page_size;
    int success = test2PageTableFifo.newEntry(pageNumber, FIFO);

    numReferencesFifo++;
    if(success == 1){
      numFaultsFifo++;
    } else if (success == 2) {
      numReplacementsFifo++;
      numFaultsFifo++;
    }
  }
  timer1 = clock()-timer1;
  cout<<"Number of References: " << numReferencesFifo << endl;
  cout<<"Number of Page Faults: " << numFaultsFifo << endl;
  cout<<"Number of Page Replacements: "<< numReplacementsFifo <<endl;
  cout << "Time Elapsed: " << ((float)timer1)/CLOCKS_PER_SEC << " seconds" << endl;
  
	cout << "****************Simulate Random replacement****************************" << endl;
	// TODO: Add your code to calculate number of page faults using Random replacement algorithm
	// TODO: print the statistics and run-time
  timer1 = clock();
  PageTable test2PageTableRand(num_pages, num_frames);
  
  int numReferencesRand{0};
  int numFaultsRand{0};
  int numReplacementsRand{0};

  ifstream large_s_rand("large_refs.txt");
  while (getline(large_s_rand, line)) {
    int pageNumber = stoi(line) / page_size;
    int success = test2PageTableRand.newEntry(pageNumber, RANDOM);

    numReferencesRand++;
    if(success == 1){
      numFaultsRand++;
    } else if (success == 2) {
      numReplacementsRand++;
      numFaultsRand++;
    }
  }
 
  cout<<"Number of References: " << numReferencesRand << endl;
  cout<<"Number of Page Faults: " << numFaultsRand << endl;
  cout<<"Number of Page Replacements: "<< numReplacementsRand <<endl;
  cout << "Time Elapsed: " << ((float)timer1)/CLOCKS_PER_SEC << " seconds" << endl;

	cout << "****************Simulate LRU replacement****************************" << endl;
	// TODO: Add your code to calculate number of page faults using LRU replacement algorithm
	// TODO: print the statistics and run-time
  timer1 = clock();
    PageTable test2PageTablelru(num_pages, num_frames);
  
  int numReferencesLRU{0};
  int numFaultsLRU{0};
  int numReplacementsLRU{0};

  ifstream large_s_lru("large_refs.txt");
  while (getline(large_s_lru, line)) {
    int pageNumber = stoi(line) / page_size;
    int success = test2PageTablelru.newEntry(pageNumber, LRU);

    numReferencesLRU++;
    if(success == 1){
      numFaultsLRU++;
    } else if (success == 2) {
      numReplacementsLRU++;
      numFaultsLRU++;
    }
  }
  timer1 = clock() - timer1;
  cout<<"Number of References: " << numReferencesLRU << endl;
  cout<<"Number of Page Faults: " << numFaultsLRU << endl;
  cout<<"Number of Page Replacements: "<< numReplacementsLRU <<endl;
  cout << "Time Elapsed: " << ((float)timer1)/CLOCKS_PER_SEC << " seconds" << endl;
}
