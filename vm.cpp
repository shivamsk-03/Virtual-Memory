#include "page.h"
#include "process.h"
#include <bits/stdc++.h>
using namespace std;

string LIST = "plist.txt";
string TRACE = "ptrace.txt";

#define MEMORYSIZE 400
vector<char> R(4), IR(4);
int IC = 0;

//string to store algorithm choice from user
string algo = "";

// pSize is parsed from pList showing each process id and it's size <<pid, size> ...>
vector<vector<int>> pSize;

// pTrace is parsed from pTrace that keeps track of all the referenced memory locations called by the system <<pid, memoryLocation>...>
vector<vector<int>> pTrace;

// processList represented all the current processes in our simulation, and is a vector of process objects created at the start of the program
vector<Process *> processList;

// mainMemory represents the main memory in this simulation, it is implemented as a deque because we will need to remove from both ends for some algorithms
deque<deque<Page *>> mainMemory;

// virtualMemory represented the virtual memory in this simulation, it is implemented as a vector of a vector of pages (pageTable) which will contain each processes' page table
vector<vector<Page *>> virtualMemory;

// pageSwap will keep count of how many page swaps are performed in the simulation
int pageSwaps = 0;

void FIFO(int sizeOfPages, int numFrames)
{
    cout << "In FIFO!" << endl;
    fstream fout;
    fout.open("output_vm.txt");
    fout.seekg(ios::beg);
    int pid;       // pid is the process id of the process "trace" is currently calling
    int memLoc;    // memLoc is the location of memory that "trace" is currently calling
    int mruLoc;    // freeLoc is the frame's index in main memory of the page that "trace" is currently calling if it is already in memory
    Page *reqPage; // reqPage is the page that "trace" is currently requesting

    for (int i = 0; i < pTrace.size(); i++)
    {
        pid = pTrace[i][0];                                       // get process id
        memLoc = pTrace[i][1];                                    // get memory location  // calculate number of frames per process
        reqPage = virtualMemory[pid][(memLoc - 1) / sizeOfPages]; // we do memLoc-1 because memory starts at 1, while our index starts at 0, so we shift it down 1 by subtracting 1

        // DEBUGGING PRINT STATEMENTS
        fout << "DEBUGGING FIFO: Size of Pages: " << sizeOfPages << endl;
        fout << "DEBUGGING FIFO: Iteration Number: " << i << endl;
        fout << "DEBUGGING FIFO: Memory Location: " << memLoc << endl;
        fout << "DEBUGGING FIFO: Size of process is: " << pSize[pid][1] << endl;
        fout << "Getting pid: " << pid << " requesting page number: " << (memLoc - 1) / sizeOfPages << endl;

        // First, we need to check if this page is already in memory or not, if not, then we need to put it in memory
        if (reqPage->validBit == 0)
        {
            fout << "Page is not in memory!" << endl;
            // If there is free space, we can simply push the page to memory, otherwise, we need to replace a frame and perform a pageswap
            if (mainMemory[pid].size() < numFrames)
            {
                mainMemory[pid].push_back(reqPage); // push reqPage to main memory
            }
            // If there is no free space, we will pop the front because the initialization of main memory loaded pages from frame 0 up to frame numFrames,
            // so the least recently used page will always be the first frame
            else
            {
                mainMemory[pid].front()->validBit = 0; // set page at front of memory to be 0, since it will no longer be in memory
                mainMemory[pid].pop_front();           // pop the first frame since it is being put in first
                mainMemory[pid].push_back(reqPage);    // push the newly requested page into main memory. The last page will be at the back
                fout << "Swapped page!" << endl;
                pageSwaps++; // increment count of page swap
            }
            reqPage->validBit = 1; // set validBit of reqPage to 1 because it is now in memory
        }
        // If page is already in memory, we do nothing
        else
        {
            fout << "Page is already in memory!" << endl;
        }
        fout << endl;
    }
    fout.close();
}

// Least recently used will loop through the trace file and begin simulation. Since main memory was initiailized with the first pSize[i]/sizeOfPages frames,
// the pages already in main memory at the start of execution were least recently used at time 0. Thus, we can just proceed by always removing the top frame
void LRU(int sizeOfPages, int numFrames)
{
    cout << "In LRU!" << endl;
    fstream fout;
    fout.open("output_vm.txt");
    fout.seekg(0, ios::beg);
    int pid;       // pid is the process id of the process "trace" is currently calling
    int memLoc;    // memLoc is the location of memory that "trace" is currently calling
    int mruLoc;    // freeLoc is the frame's index in main memory of the page that "trace" is currently calling if it is already in memory
    Page *reqPage; // reqPage is the page that "trace" is currently requesting

    for (int i = 0; i < pTrace.size(); i++)
    {
        pid = pTrace[i][0];                                       // get process id
        memLoc = pTrace[i][1];                                    // get memory location  // calculate number of frames per process
        reqPage = virtualMemory[pid][(memLoc - 1) / sizeOfPages]; // we do memLoc-1 because memory starts at 1, while our index starts at 0, so we shift it down 1 by subtracting 1

        // DEBUGGING PRINT STATEMENTS
        fout << "DEBUGGING LRU: Size of Pages: " << sizeOfPages << endl;
        fout << "DEBUGGING LRU: Iteration Number: " << i << endl;
        fout << "DEBUGGING LRU: Memory Location: " << memLoc << endl;
        fout << "DEBUGGING LRU: Size of process is: " << pSize[pid][1] << endl;
        fout << "Getting pid: " << pid << " requesting page number: " << (memLoc - 1) / sizeOfPages << endl;

        // First, we need to check if this page is already in memory or not, if not, then we need to put it in memory
        if (reqPage->validBit == 0)
        {
            fout << "Page is not in memory!" << endl;
            // If there is free space, we can simply push the page to memory, otherwise, we need to replace a frame and perform a pageswap
            if (mainMemory[pid].size() < numFrames)
            {
                mainMemory[pid].push_back(reqPage); // push reqPage to main memory
            }
            // If there is no free space, we will pop the front because the initialization of main memory loaded pages from frame 0 up to frame numFrames,
            // so the least recently used page will always be the first frame
            else
            {
                mainMemory[pid].front()->validBit = 0; // set page at front of memory to be 0, since it will no longer be in memory
                mainMemory[pid].pop_front();           // pop the first frame since it is the least recently used
                mainMemory[pid].push_back(reqPage);    // push the newly requested page into main memory. The most recently used pages will be at the back
                fout << "Swapped page!" << endl;
                pageSwaps++; // increment count of page swap
            }
            reqPage->validBit = 1; // set validBit of reqPage to 1 because it is now in memory
        }
        // If page is already in memory, we can place it at the back of the frame table so the most recently used frames will always be at the back of
        // the frame table in main memory
        else
        {
            fout << "Page is already in memory!" << endl;
            // First, we need to determine where in main memory the page that "trace" is currently calling is
            for (int j = 0; j < mainMemory[pid].size(); j++)
            {
                // If the pageId of the frame we are currently searching at is equal to the pageId of the page "trace" is currently calling,
                // we have found the position of the frame
                if (mainMemory[pid][j]->pageId == reqPage->pageId)
                {
                    mruLoc = j;
                    break;
                }
            }

            mainMemory[pid].erase(mainMemory[pid].begin() + mruLoc); // we will then erase the frame in main memory, shifting the indexes
            mainMemory[pid].push_back(reqPage);                      // we will then push back the requested page from trace
            fout << "Page swapped at the last of memory!" << endl;
        }
        fout << endl;
    }
    fout.close();
}

int VM(string a, string b, string c, string d)
{
    cout << "Virtual Memory Called." << endl;
    int sizeOfPages;        // sizeOfPages represents the size of each frame and page
    string pageReplaceAlgo; // algorithm represents the page replacement algorithm input by the user
    istringstream iss;      // iss is the stringstream object to convert the arguments to strings

    // argument variables
    int processId;  // processId represents the process id in plist
    int proMemSize; // proMemSize represents the amount of memory for the process
    int proMemLoc;  // proMemLoc represents the memory of location that the system requests access to

    // initialization variables
    int mainMemFrames; // mainMemFrames represents the number of page frames in main memory
    int numFrames;     // numFrames represents the number of page frames per process in main memory at initialization

    ifstream in_1(a);
    if (!(in_1))
    {
        cerr << "Error: Please input a valid plist file!" << endl;
        return 1;
    }
    // parse through plist, storing each process id and its memory size in vector "process", then pushing it to pSize in the form <pid, memsize>
    while (in_1 >> processId >> proMemSize)
    {
        vector<int> process;
        process.push_back(processId);
        process.push_back(proMemSize);
        pSize.push_back(process);
    }
    // grab ptrace as an argument, if null, then prompt user to input ptrace file
    ifstream in_2(b);
    if (!(in_2))
    {
        cerr << "Error: Please input a valid ptrace file!" << endl;
        return 1;
    }
    // parse through ptrace, storing the series of memory access in vector "memLocAccess", then pushing it to pTrace in the form <pid, memLocation>
    while (in_2 >> processId >> proMemLoc)
    {
        vector<int> memLocAccess;
        memLocAccess.push_back(processId);
        memLocAccess.push_back(proMemLoc);
        pTrace.push_back(memLocAccess);
    }
    // check validity of argument 3 (size of pages), convert it to integer and store in variable "sizeOfPages"
    sizeOfPages = stoi(c);
    if (sizeOfPages <= 0)
    {
        cerr << "Error: Please input a valid positive integer for size of pages!" << endl;
        return 1;
    }
    // check validity of argument 4 (page replacement algorithm) that it is a string and matches one of the existing algorithms
    iss.str(d);
    if (!(iss >> pageReplaceAlgo))
    {
        cerr << "Error: Please input a valid string argument for page replacement algorithm!" << endl;
        return 1;
    }
    iss.clear();
    cout << pageReplaceAlgo << endl;
    if ((pageReplaceAlgo != "FIFO") && (pageReplaceAlgo != "LRU"))
    {
        cerr << "Error: Please input a valid page replacement algorithm!" << endl;
        return 1;
    }

    // loop through all the process sizes and create a new process object, and push it to processList
    for (int i = 0; i < pSize.size(); i++)
    {
        proMemSize = pSize[i][1];
        processList.push_back(new Process(i, proMemSize, sizeOfPages));
    }

    // initialize virtual memory by pushing each process' page table to virtualMemory
    for (int i = 0; i < processList.size(); i++)
    {
        for (int j = 0; j < MEMORYSIZE / sizeOfPages / processList.size(); j++)
        {
            processList[i]->pageTable[j]->validBit = 1;
        }
        virtualMemory.push_back(processList[i]->pageTable);
    }

    // set values of mainMemFrames and numFrames
    mainMemFrames = MEMORYSIZE / sizeOfPages; //80
    numFrames = mainMemFrames / processList.size();      //16
    // initialize equal number of page frames for each process into main memory
    for (int i = 0; i < processList.size(); i++)
    {
        deque<Page *> frames;
        // for each process, put equal amount of frames from virtual memory into "frames"
        for (int j = 0; j < numFrames; j++)
        {
            frames.push_back(virtualMemory[i][j]);
        }
        // after equal amount of frames have ben loaded to "frames", load it into main memory
        mainMemory.push_back(frames);
        frames.clear();
    }

    if (pageReplaceAlgo == "FIFO")
    {
        FIFO(sizeOfPages, numFrames);
        cout << "Page swaps: " << pageSwaps << endl;
    }
    else if (pageReplaceAlgo == "LRU")
    {
        LRU(sizeOfPages, numFrames);
        cout << "Page swaps: " << pageSwaps << endl;
    }
}

void executeUserProgram()
{
    VM(LIST, TRACE, "5", algo);

    cout << "\n\n #############   Program DONE   ########### \n\n";
}

void LOAD()
{
    cout<<"Enter the algorithm: FIFO/ LRU:"<<endl;
    cin >> algo;
    executeUserProgram();
}

int main()
{
    LOAD();
    return 0;
}