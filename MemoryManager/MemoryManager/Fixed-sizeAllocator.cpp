#include <iostream>
#include <windows.h>


class FSA 
{
private:
#ifdef _DEBUG
	int initbusyblocks;
	int initfreeblocks;
#endif
	size_t size;//������ ��������
	size_t numPage;//���-�� ��������� � ��������
	struct Page
	{
		Page* nextPage;
		int fh;
		size_t numInit;
		void* data;
	};
	Page* page;
	
public:

	FSA() {};

	void init(size_t size, size_t numPage)
	{
		this->size = size;
		this->numPage = numPage;
#ifdef _DEBUG
		initbusyblocks = 0;
		initfreeblocks = 10;
#endif
		void* pBuf = VirtualAlloc(NULL, size * numPage + sizeof(Page), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		//��������� ������ �������� � ��������� ������ �� ���
		page = (Page*)pBuf;
		page->nextPage = nullptr;
		page->numInit = 0;
		page->fh = -1;
		page->data = (char*)pBuf + sizeof(Page);
	}

	void* alloc()
	{
		Page* pg = page;
		while (pg->fh==-1)
		{
			//�������� �� ���-�� ������������������ ������
			if (pg->numInit < numPage) 
			{
				void* p = (char*)(pg->data) + pg->numInit * size;
				pg->numInit++;
#ifdef _DEBUG
				initbusyblocks++;
				initfreeblocks--;
#endif
				return p;
			}
			else if(pg->nextPage==nullptr)
			{
				//���������� ��������
				void* pBuf = VirtualAlloc(NULL, size * numPage + sizeof(Page), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
				//��������� ������ �������� � ��������� ������ �� ���
				pg->nextPage = (Page*)pBuf;
				(pg->nextPage)->nextPage = nullptr;
				(pg->nextPage)->numInit = 0;
				(pg->nextPage)->fh = -1;
				(pg->nextPage)->data = (char*)pBuf + sizeof(Page);
				void* p = (char*)(pg->data) + pg->numInit * size;
				pg->numInit++;
#ifdef _DEBUG
				initbusyblocks++;
				initfreeblocks+=9;
#endif
				return p;
			}
			pg = pg->nextPage;
		}
		void* p = (char*)(pg->data) + pg->fh * size;
		pg->numInit++;
#ifdef _DEBUG
		initbusyblocks++;
		initfreeblocks--;
		initfreeblocks--;
#endif
		pg->fh = *(int*)p;//���������� ��������� ��������� ���� � ������
		return  p;
	}

	bool free(void* p)
	{
		Page* pg = page;
		while (pg != nullptr) 
		{
			if ((void*)(pg->data) <= p && (void*)((char*)(pg->data) + numPage * size) > p) 
			{
				*(int*)(p) = pg->fh;//���������� ��������� ��������� ���� � ������������� ����
				pg->fh = (int)(((char*)(p) - (char*)(pg->data)) / size);//��������� � ������ ������ ��������� ������ ����� �������������� �����
#ifdef _DEBUG
				initfreeblocks++;
				initbusyblocks--;
#endif
				return true;
			}
			pg = pg->nextPage;
		}
		return false;
	}

	void destroy() 
	{
		while (page->nextPage != nullptr) 
		{
			VirtualFree((void*)(page), 0, MEM_RELEASE);
			if (page->nextPage == nullptr) break;
			else page = page->nextPage;
		}
		VirtualFree((void*)(page), 0, MEM_RELEASE);
		page = nullptr;
	}

#ifdef _DEBUG
	void dumpStat() const 
	{
		std::setlocale(LC_ALL, "rus");
		std::cout << "\n\t���������� �������, ��������� ������ FSA"<< size << ": " << initbusyblocks << ", " << initfreeblocks<< "\n\n\n";
	}
#endif
	
#ifdef _DEBUG
	void dumpBlocks() const
	{
		std::cout << "\n\tC����� ������, ����������� � �� � �� ������� FSA: ";
		std::cout << "\n\t|\t����� �����\t\t|\n";
		Page* pg = page;
		while (pg != nullptr)
		{
			pg->data;
			for (int i = 0; i < pg->numInit;i++)
			{
				std::cout << "\t|\t" << (void*)((char*)pg->data + i * size) << "\t|\t\n";
			}
			pg = pg->nextPage;
		}
		std::cout << "\n\n\n";
	}
#endif
};