#ifndef MEMCHECK_H
#define MEMCHECK_H
#include <iostream>

///////////////////////////////////
//                               //
// Sample Programs with MemCheck //
//                               //
///////////////////////////////////

/*
struct Object {
    int x, y, z;
    Object(): x(0), y(0), z(0) {}
};
int main() {
    Object* o = new Object[3];
    fullProgMem.memStats();
    delete[] o;
    fullProgMem.memStats();
}

// For more specific behavior, you can create your own MemCheck objects.
// You can enable and disable them whenever you chose, and disable program-end 
// display of heap status.
// Ex)
    MemCheckDisplay(false);    <- No display of heap status on program end
    MemCheck m;
    int* arr = new int[4];
    m.disable();               <- Disable heap monitoring by m
    double* x = new double[3]; <- This is not counted by m 
*/


// Class that stores information about heap allocation
class MemCheck {
    private:
        size_t _numAlloc;      // Number of total heap allocations
        size_t _numDealloc;    // Number of total heap deallocations
        size_t _bytesAlloc;    // Total bytes allocated on heap
        size_t _bytesDealloc;  // Total bytes deallocated on heap
        size_t _currByteCount; // Curr bytes allocated on the heap
        bool _enabled;         // If enabled, a MemCheck object tracks heap usage 
    public:
        MemCheck();
        ~MemCheck();

        // Function that prints memory information
        void memStats() {
            std::cout << "[Number of Allocations]: " << _numAlloc << std::endl;
            std::cout << "[Bytes Allocated]: " << _bytesAlloc << std::endl;
            std::cout << "[Number of Deallocations]: " << _numDealloc << std::endl;
            std::cout << "[Bytes Deallocated]: " << _bytesDealloc << std::endl;
            std::cout << "[Current Bytes Allocated]: " << _currByteCount << std::endl << std::endl;
        }

        // Enable heap memory tracking
        void enable() { _enabled = true; }

        // Disable heap memory tracking
        void disable() { _enabled = false; }

        // Getters
        size_t numAlloc() { return _numAlloc; }
        size_t numDealloc() { return _numDealloc; }
        size_t bytesAlloc() { return _bytesAlloc; }
        size_t bytesDealloc() { return _bytesDealloc; }
        size_t currByteCount() { return _currByteCount; }
        bool enabled() { return _enabled; }

        friend void* operator new (size_t size);
        friend void* operator new[] (size_t size);
        friend void operator delete (void* memory, size_t size);
        friend void operator delete[] (void* memory, size_t size);
};

// Maximum number of MemChecks for a program is set to 30 by default
// Change the macro definition if needed
#ifndef MAX_MEMCHECKS
#define MAX_MEMCHECKS 30
#endif

// Global memory check for the life of the program
// Using enable and disable with fullProgMem may lead to unintended behavior
static MemCheck fullProgMem;
static bool displayOnDestroy = true; // Display heap status on program termination
static MemCheck* mems[MAX_MEMCHECKS];
static size_t memCount = 0;

// Use to change if message should be printed after program terminates
void MemCheckDisplay(bool b);

MemCheck::MemCheck(): 
    _numAlloc(0), 
    _numDealloc(0), 
    _bytesAlloc(0), 
    _bytesDealloc(0), 
    _currByteCount(0), 
    _enabled(true) 
{
    if (memCount == MAX_MEMCHECKS) {
        throw std::runtime_error("Exceeds maximum number of MemChecks");
    }

    mems[memCount] = this;
    memCount++;     
}

MemCheck::~MemCheck() {
    // Only display end behavior if for global MemCheck and display is enabled
    if (this != &fullProgMem || !displayOnDestroy) {
        return;
    }

    // Display memory status on program end
    if (_currByteCount) {
        std::cout << "\033[31m[Memory Not Freed]: " << _currByteCount << " bytes not freed" << std::endl;
    } else {
        std::cout << "\033[32m[All Memory Freed]" << std::endl;
    }
}

// Use to change if message should be printed after program terminates
void MemCheckDisplay(bool b) {
    displayOnDestroy = b;
}

void* operator new (size_t size) {
    if (size == 0) {
        size++; // Avoid std::malloc(0) which may return nullptr on success
    }

    // Update info for all enabled MemChecks
    for (size_t i = 0; i < memCount; i++) {
        if (mems[i]->enabled()) {
            mems[i]->_numAlloc++;
            mems[i]->_bytesAlloc += size;
            mems[i]->_currByteCount += size;
        }
    }

    if (void *ptr = std::malloc(size)) {
        return ptr;
    }

    throw std::bad_alloc{}; // Required by [new.delete.single]/3
}

void* operator new[] (size_t size) {
    if (size == 0) {
        size++; // Avoid std::malloc(0) which may return nullptr on success
    }

    // Update info for all enabled MemChecks
    for (size_t i = 0; i < memCount; i++) {
        if (mems[i]->enabled()) {
            mems[i]->_numAlloc++;
            mems[i]->_bytesAlloc += size;
            mems[i]->_currByteCount += size;
        }
    }

    if (void *ptr = std::malloc(size)) {
        return ptr;
    }

    throw std::bad_alloc{}; // Required by [new.delete.single]/3
}

void operator delete (void* memory, size_t size) {
    // Update info for all enabled MemChecks
    for (size_t i = 0; i < memCount; i++) {
        if (mems[i]->enabled()) {
            mems[i]->_numDealloc++;
            mems[i]->_bytesDealloc += size;
            mems[i]->_currByteCount -= size;
        }
    }

    std::free(memory);
}

void operator delete(void* memory) { std::free(memory); }

void operator delete[] (void* memory, size_t size) {
    // Update info for all enabled MemChecks
    for (size_t i = 0; i < memCount; i++) {
        if (mems[i]->enabled()) {
            mems[i]->_numDealloc++;
            mems[i]->_bytesDealloc += size;
            mems[i]->_currByteCount -= size;
        }
    }

    std::free(memory);
}

void operator delete[] (void* memory) { std::free(memory); }

#endif
