#ifndef PROCESS_H_ // prevents inclusion of same header file multiple times
#define PROCESS_H_

#include "page.h"
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>  //manipulate output of cpp program

using namespace std;

class Process {

public:
    int pid; // pid of process
    int size;   // size of memory for process
    int numPages;   // number of page entries in process' page table
    vector<Page*> pageTable;    // page table associated with process
    
    Process (int pid, int sz, int sizeOfPages) {
        pid = pid;
        size = sz;
        numPages = size/sizeOfPages;
        // if there is extra memory remaining, create an extra page to accomodate for the extra memory
        if (size%sizeOfPages>0) {
            numPages++;
        }
        for (int i = 0; i < numPages; i++) {
            Page* newPage = new Page(pid, i, 0);
            pageTable.push_back(newPage);
        }
    }
};

#endif /* PROCESS_H_ */
