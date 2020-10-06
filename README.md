# StringPrinter
This program is an assignment which demostrates synchronization between threads using C++ mutex, unique_lock and condition variable.

Steps build and run the executable 
1. Make sure you have of cmake(version 3.18.2) on your machine and make sure gcc is installed and 	   both g++ and gcc are under `/usr/bin/` directory. 
2. Change the working directory to Problem2 directory
3. create build directory under Problem2 dir
4. Navigate to build directory
5. Run `cmake ..` command
6. Run `cmake --build . --target StringPrinter` command to build exec.
7. Run the exec `StringPrinter` which will be under build/StrinPrinter directory


TO-DO's
1. Make changes in the cmake file to add binary to a separate directory
2. Debug segfault when we cancel the thread
