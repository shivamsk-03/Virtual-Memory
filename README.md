# Virtual Memory Multiprogrammable OS
This is a Virtual Memory Multiprogrammable OS that uses the First-In-First-Out (FIFO) and Least Recently Used (LRU) page replacement algorithms.

# Introduction
- Virtual memory is a feature of an operating system that enables a computer to be able to compensate shortages of physical memory by temporarily transferring pages of data from random access memory (RAM) to disk storage. This allows a computer to run larger applications or multiple applications simultaneously, and is an essential feature in modern operating systems.

- This project simulates the virtual memory management system of an operating system, specifically the page replacement algorithm. The simulator implements two algorithms - FIFO and LRU - to manage page replacement.

# Installation
To use the simulator, download the files and compile the `main.cpp` file using any C++ compiler.

# Usage
To use the simulator, run the compiled executable and input the following when prompted:

- The page size in bytes
- The number of frames in main memory
- The algorithm to use for page replacement (FIFO or LRU)
- The name of the file containing the process information `(plist.txt)`
- The name of the file containing the process trace `(ptrace.txt)`
- The output of the simulator will be written to a file called `output_vm.txt`.

# File structure
`vm.cpp`: The main C++ source file containing the implementation of the virtual memory simulator

`page.h`: The header file defining the Page class used to represent a page in virtual memory

`process.h`: The header file defining the Process class used to represent a process in the simulator

`plist.txt`: The file containing information about the processes in the simulator (process ID and size)

`ptrace.txt`: The file containing the memory locations accessed by each process in the simulator

`output_vm.txt`: The file containing the output of the simulator

# Implementation details
The virtual memory simulator uses a deque to represent the main memory and a vector of vectors to represent the virtual memory. Each process in the simulator is represented by a Process object containing information such as its ID, size, and page table.

When a memory location is accessed by a process, the simulator first checks if the requested page is already in main memory. If it is, nothing happens. If it is not, the simulator checks if there is free space in main memory. If there is, the requested page is loaded into main memory. If there is no free space, the simulator replaces the least recently used page in main memory with the requested page.

# Page replacement algorithms
- ## First-In-First-Out (FIFO)
The FIFO algorithm replaces the page that has been in main memory the longest. To implement this algorithm, the simulator uses a deque to represent main memory and always replaces the page at the front of the deque.

- ## Least Recently Used (LRU)
The LRU algorithm replaces the page that has not been used for the longest time. To implement this algorithm, the simulator uses a deque to represent main memory and always replaces the page at the front of the deque, which is the least recently used page. To keep track of when a page was last used, the simulator updates a counter for each page every time it is accessed. The page with the lowest counter value is the least recently used page.
