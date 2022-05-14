# MemCheck
A heap memory tool designed for C++

I designed this tool to strengthen my skills with dynamic memory and debugging. The tool stores heap usage stats, and can be used to analyze how much memory is being used, and if there are any memory leaks. 

## Using MemCheck:

All you need to do is include the MemoryCheck.h file in the file containing your main function. The MemCheck object exists as static, and will remain so for the length of the program. You can print out the current statistics of heap usage with ```mc.memStats()```, which prints the number of allocations, the bytes allocated, the number of deallocations, the bytes deallocated, and the current remaining bytes on the heap. The MemCheck object also has a destructor, which prints whether all heap memory has been freed or not. If there is remaining memory at the end of the main, the remaining bytes will be printed. Because MemCheck is a struct, the individual members can be checked explicity if you would like to analyze only specific stats.
