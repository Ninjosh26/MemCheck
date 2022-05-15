# MemCheck
A heap memory tool designed for C++

I designed this tool to strengthen my skills with dynamic memory and debugging. The tool stores heap usage stats, and can be used to analyze how much memory is being used, and if there are any memory leaks. 

## Using MemCheck:

All you need to do is add the MemCheck.h file into your current directory, and include MemCheck.h in the file containing your main function. The MemCheck object ```fullProgMem``` is static, so it will not be reinstantiated after the program starts. This instance serves to observe heap usage across the length of the entire program.

You can print out the current statistics of heap usage for a MemCheck object by calling the ```memStats()``` function on the desired instance, which prints the number of allocations, the bytes allocated, the number of deallocations, the bytes deallocated, and the current remaining bytes on the heap observed by the MemCheck object. The MemCheck object also has a destructor, which prints whether all heap memory has been freed or not for the ```fullProgMem``` instance. If there is remaining memory at the end of the main, the remaining bytes will be printed. 

Because MemCheck is a class, the individual members can be checked using the provided getter functions. This is to prevent any unwanted altering of the MemCheck objects. The provided getters are: ```numAlloc()```, ```numDealloc()```, ```bytesAlloc()```, ```bytesDealloc()```, ```currByteCount()```, and ```enabled()```.

If you do not want the remaining heap memory to be printed upon termination of the program, you can use ```MemCheckDisplay(false);``` to disable the message displayed when the program ends.

You can create new MemCheck objects in your program with ```MemCheck [NAME];```. Each MemCheck object can be enabled and disabled to observe select parts of a program using the ```enable()``` and ```disable()``` functions, although it is recommended you do not use them on ```fullProgMem``` as it might cause inaccurate observations of the overall heap usage. The macro ```MAX_MEMCHECKS``` sets the limit for total MemCheck instance for the program, and it is set by default to 30. This limit can be changed by altering the macro definition to the desired maximum.
