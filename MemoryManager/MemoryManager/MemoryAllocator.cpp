#include <iostream>
#include <windows.h>
#include "Fixed-sizeAllocator.cpp"
#include "CoalesceAllocation.cpp"
#include <cassert>

class MemoryAllocator 
{
private:

	FSA fsa16, fsa32, fsa64, fsa128, fsa256, fsa512;
	CoalesceAllocator ca;
	bool isInit;
public:
	//создание неинициализированного аллокатора
	MemoryAllocator() 
	{
#ifdef _DEBUG
		isInit = false;
#endif
	};

	//освобождение всех блоков памяти, запрошенных у ОС
	virtual ~MemoryAllocator() 
	{
#ifdef _DEBUG
		assert(isInit == false);
#endif
	};

	//инициализация нужного аллокатора
	virtual void init() 
	{
#ifdef _DEBUG
		assert(isInit == false);
#endif
		fsa16.init(16,10);
		fsa32.init(32,10);
		fsa64.init(64,10);
		fsa128.init(128,10);
		fsa256.init(256,10);
		fsa512.init(512,10);
		ca.init(10485760,10);
#ifdef _DEBUG
		isInit = true;
#endif
	};

	//деинициализация аллокатора
	virtual void destroy() 
	{
#ifdef _DEBUG
		assert(isInit == true);
#endif
		ca.destroy();
		fsa16.destroy();
		fsa32.destroy();
		fsa64.destroy();
		fsa128.destroy();
		fsa256.destroy();
		fsa512.destroy();
#ifdef _DEBUG
		isInit == false;
#endif
	};

	//выделение блока памяти размером size
	virtual void* alloc(int size) 
	{
		if(size<16) return fsa16.alloc();
		else if(size<32) return fsa32.alloc();
		else if (size<64) return fsa64.alloc();
		else if(size<128) return fsa128.alloc();
		else if(size<256) return fsa256.alloc();
		else if(size<512) return fsa512.alloc();
		else if(size< 10485760) return ca.alloc(size);
		else
		{
			return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		}

	};

	//освобождение памяти, на которую указывает p
	virtual void free(void* p) 
	{
#ifdef _DEBUG
		assert(isInit == true);
#endif // _DEBUG
		if (fsa16.free(p)) return;
		else if (fsa32.free(p)) return;
		else if (fsa64.free(p)) return;
		else if (fsa128.free(p)) return;
		else if (fsa256.free(p)) return;
		else if (fsa512.free(p)) return;
		else if (ca.free(p)) return;
		else
		{
			VirtualFree(p, 0, MEM_RELEASE);
			return;
		}
	};

#ifdef _DEBUG
	//статистика по кол-ву занятых и свободных блоков и пр.
	virtual void dumpStat() const 
	{
		fsa16.dumpStat();
		fsa32.dumpStat();
		fsa64.dumpStat();
		fsa128.dumpStat();
		fsa256.dumpStat();
		fsa512.dumpStat();
		ca.dumpStat();
	};
	//вывод списка всех занятых блоков : их адреса и размеры
	virtual void dumpBlocks() const 
	{
		fsa16.dumpBlocks();
		fsa32.dumpBlocks();
		fsa64.dumpBlocks();
		fsa128.dumpBlocks();
		fsa256.dumpBlocks();
		fsa512.dumpBlocks();
		ca.dumpBlocks();
	};
#endif
};