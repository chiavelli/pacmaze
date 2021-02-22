# pacmaze

This is an implementation of the "find closest dot" problem discussed during Friday's call.

In order to test the implementation, I have implemented a setup which allows for reading in maze configurations from text files. So long as the input files are valid an attempt will be made to find the closest dot.

This was compiled with g++ 8.1.0, which comes packaged with MinGW-W64:
g++ --version
g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

To compile this project with g++, use the following command:
g++ -o findClosest main.cpp Cell.cpp Grid.cpp
