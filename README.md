# assignment2OS

This is an example of the bankers algorithm, the second assignment for my operating systems class.
Based on the given data in the input.txt file there are processes with their required resources as well as the number of available resources for each resource listed.
This allows a good representation of multithreading and determining an algorithm to figure out what the safe sequence to run them is based on what is available.

It is compiled using the following commands:

clang++ bankers.cpp 
./a.out bankers.cpp input.txt
