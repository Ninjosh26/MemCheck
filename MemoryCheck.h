#ifndef MEMORYCHECK_H
#define MEMORYCHECK_H
#include <iostream>

// Struct that stores information about heap allocation
struct MemCheck {
    size_t numAlloc = 0; // Number of total heap allocations
    size_t numDealloc = 0; // Number of total heap deallocations
    size_t bytesAlloc = 0; // Total bytes allocated on heap
    size_t bytesDealloc = 0; // Total bytes deallocated on heap
    size_t currByteCount = 0; // Curr bytes allocated on the heap
    bool displayOnDestroy = true; // Defaults to display heap memory remaining on program termination

    // Function that prints memory information
    void memStats() {
        std::cout << "[Number of Allocations]: " << numAlloc << std::endl;
        std::cout << "[Bytes Allocated]: " << bytesAlloc << std::endl;
        std::cout << "[Number of Deallocations]: " << numDealloc << std::endl;
        std::cout << "[Bytes Deallocated]: " << bytesDealloc << std::endl;
        std::cout << "[Current Bytes Allocated]: " << currByteCount << std::endl << std::endl;
    }

    // Use to change if message should be printed after program terminates
    void display(bool b) {
        displayOnDestroy = b;
    }

    ~MemCheck() {
        if (!displayOnDestroy) {
            return;
        }

        if (currByteCount) {
            std::cout << "\033[31m[Memory Not Freed]: " << currByteCount << " bytes not freed" << std::endl;
        } else {
            std::cout << "\033[32m[All Memory Freed]" << std::endl;
        }
    }
};

static MemCheck mc;

void* operator new (size_t size) {
    mc.numAlloc++;
    mc.bytesAlloc += size;
    mc.currByteCount += size;

    if (size == 0) {
        size++; // avoid std::malloc(0) which may return nullptr on success
    }

    if (void *ptr = std::malloc(size)) {
        return ptr;
    }

    throw std::bad_alloc{}; // required by [new.delete.single]/3
}

void* operator new[] (size_t size) {
    mc.numAlloc++;
    mc.bytesAlloc += size;
    mc.currByteCount += size;

    if (size == 0) {
        size++; // avoid std::malloc(0) which may return nullptr on success
    }

    if (void *ptr = std::malloc(size)) {
        return ptr;
    }

    throw std::bad_alloc{}; // required by [new.delete.single]/3
}

void operator delete (void* memory, size_t size) {
    mc.numDealloc++;
    mc.bytesDealloc += size;
    mc.currByteCount -= size;

    std::free(memory);
}

void operator delete(void* memory) { std::free(memory); }

void operator delete[] (void* memory, size_t size) {
    mc.numDealloc++;
    mc.bytesDealloc += size;
    mc.currByteCount -= size;

    std::free(memory);
}

void operator delete[] (void* memory) { std::free(memory); }

//////////////////////////////////
//                              //
// Sample Program with MemCheck //
//                              //
//////////////////////////////////

/*

struct Object {
    int x, y, z;

    Object(): x(0), y(0), z(0) {}
};

int main() {
    Object* o = new Object[3];

    mc.memStats();

    delete[] o;

    mc.memStats();
}

*/

#endif
