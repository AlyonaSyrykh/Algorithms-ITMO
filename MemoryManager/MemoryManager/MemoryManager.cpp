#include <iostream>
#include "MemoryAllocator.cpp"
#include <vector>

int main()
{
    MemoryAllocator allocator;
    allocator.init();
    std::vector<int*> fsa16,fsa32,fsa64, fsa128, fsa256, fsa512, ca;
    for (int i = 0; i < 20; i++) 
    {
        fsa16.push_back((int*)allocator.alloc(sizeof(int)));
        fsa32.push_back((int*)allocator.alloc(5*sizeof(int)));
        fsa64.push_back((int*)allocator.alloc(10*sizeof(int)));
        fsa128.push_back((int*)allocator.alloc(20*sizeof(int)));
        fsa256.push_back((int*)allocator.alloc(50*sizeof(int)));
        fsa512.push_back((int*)allocator.alloc(100*sizeof(int)));
        ca.push_back((int*)allocator.alloc(10000*sizeof(int)));
    }
#ifdef _DEBUG
    allocator.dumpStat();
    allocator.dumpBlocks();
#endif
    
    for (int i = 0; i < 20; i++) 
    {
        allocator.free(fsa16[i]);
        allocator.free(fsa32[i]);
        allocator.free(fsa64[i]);
        allocator.free(fsa128[i]);
        allocator.free(fsa256[i]);
        allocator.free(fsa512[i]);
        allocator.free(ca[i]);
    }
    allocator.destroy();
}

