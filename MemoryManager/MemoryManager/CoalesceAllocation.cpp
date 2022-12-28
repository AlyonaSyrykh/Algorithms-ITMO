#include <iostream>
#include <windows.h>

class CoalesceAllocator
{

private:
	size_t size;
	size_t numPage;
#ifdef _DEBUG
	int initfreeblocks;
	int initbusyblocks;
#endif

	struct BlockData
	{
		BlockData* next;
		BlockData* left;
		bool isBusy;
	};
	struct FreeList {
		FreeList* next;
		FreeList* left;
	};
	struct Page
	{
		Page* nextPage;
		FreeList* freelist;
	};
	Page* page;
public:

	void init(size_t size, size_t numPage)
	{
		this->numPage = numPage;
		this->size = size;
#ifdef _DEBUG
		initbusyblocks = 0;
		initfreeblocks = 1;
#endif
		this->page = allocPage();
	}

	Page* allocPage() 
	{
		void* pBuf = VirtualAlloc(NULL, size * numPage, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		//��������� ������ �������� � ��������� ������ �� ���
		page = (Page*)pBuf;// ����������� ��������� ������ �������� �� pBuf
		page->nextPage = nullptr;
		//�������� ��� �����
		BlockData* block = (BlockData*)((char*)page + sizeof(Page));
		block->next = nullptr;
		block->left = nullptr;
		block->isBusy = false;
		//�������� ��� ��������
		page->freelist = (FreeList*)((char*)block + sizeof(BlockData));
		page->freelist->next = nullptr;
		page->freelist->left = nullptr;
		return page;
	}

	void* alloc(size_t size) 
	{
		Page* pg = page;
		while (pg != nullptr) 
		{
			FreeList* list = pg->freelist;
			if (list != nullptr) //���������, ���� �� � ������ ��������� ������ ���� ������� �������
			{
				BlockData* data = (BlockData*)((char*)list - sizeof(BlockData));
				if ((char*)data->next - (char*)data >= size)//����� �����, � ������� ����� ��������� ������ ���������������� �������
				{
					data->isBusy = true;
					if ((char*)data->next - (char*)data >= size + 512 + sizeof(BlockData)) //�������� �� ����������� ���������� ����� �� ��������� ������
					{
						BlockData *bd = (BlockData*)(data + size);
						bd->isBusy = false;
						bd->left = data;
						bd->next = data->next;//������� ����� ����
						if(data->next != nullptr) (data->next)->left = bd;//��������� �� ����� ���� �������� ���������� ����� ��� �� ����������
						else data->next = bd; // ��������� �� ����� ���� ��� ��������� � �������
						//������� �� �������� ���� ���� � ��������� ����� ��� �����������
						FreeList* fr = (FreeList*)((char*)data->next + sizeof(BlockData)); //��������� �� ����� ���� ������
						if (list->left == nullptr)//���� ������� ������
						{
							fr->left = nullptr;
							if (list->next == nullptr) fr->next = nullptr; //������������ �� ��� ��� �������
							else
							{
								fr->next = list->next;
								(list->next)->left = fr;
							}
							//���������� � ������ ��������� �� ����.��������� ����
							pg->freelist = fr;
						}
						else
						{
							fr->left = list->left;
							fr->next = list->next;
							(fr->left)->next = fr;
							(fr->next)->left = fr;
						}
						#ifdef _DEBUG
						initbusyblocks++;
						#endif
						return data;
					}
					else //������ ���� 
					{
						if (list->left == nullptr)//���� ������� ������
						{
							if (list->next == nullptr) list = nullptr; //������������ �� ��� ��� �������
							else (list->next)->left = list->left;
							pg->freelist = list;
						}
						else
						{
							(list->left)->next = list->next;
							(list->next)->left = list->left;
						}
						return data;
					}
				}
				list = list->next;
			}
			pg = pg->nextPage;
		}
		pg->nextPage = allocPage();
		BlockData* data = (BlockData*)((char*)(pg->nextPage) + sizeof(Page));
		*(bool*)(data->isBusy) = true;
		return (char*)pg->nextPage + sizeof(Page);
	}

	bool free(void* p) 
	{
		Page* pg = page;
		do
		{
			if ((void*)((char*)pg + sizeof(Page)) <= p && (void*)((char*)(pg) + numPage * size) > p)
			{
				((BlockData*)p)->isBusy = false;//����������, ��� ���� ������ ��������
				if (((BlockData*)p)->next != nullptr || ((BlockData*)p)->left != nullptr) 
				{

					//�������� ������� ������
					if (((BlockData*)p)->next != nullptr) 
					{
						if ((((BlockData*)p)->next)->isBusy == false)
						{
							//����������� ������ ������� �������� �������� � ��� �������
							((FreeList*)((char*)p + sizeof(BlockData)))->next = ((FreeList*)(((char*)((BlockData*)p)->next) + sizeof(BlockData)))->next;
							((FreeList*)((char*)p + sizeof(BlockData)))->left = ((FreeList*)(((char*)((BlockData*)p)->next) + sizeof(BlockData)))->left;
							if (((FreeList*)(((char*)p) + sizeof(BlockData)))->left == nullptr) 
							{
								pg->freelist = (FreeList*)(((char*)p) + sizeof(BlockData));
							}
							else 
								//�������� �������� � ������ � ����� ������ ������� ������� ��������
								(((FreeList*)((char*)p + sizeof(BlockData)))->left)->next = (FreeList*)(((char*)p) + sizeof(BlockData));
							if(((FreeList*)((char*)p + sizeof(BlockData)))->next!=nullptr)
								(((FreeList*)((char*)p + sizeof(BlockData)))->next)->left = (FreeList*)(((char*)p) + sizeof(BlockData));
							if(((BlockData*)p)->next!=nullptr) (((BlockData*)p)->next) = (((BlockData*)p)->next)->next;
#ifdef _DEBUG
							initbusyblocks--;
#endif
						}
						else if(((BlockData*)p)->left == nullptr || (((BlockData*)p)->left)->isBusy == true)
						{
							FreeList* fr = (FreeList*)((char*)p + sizeof(BlockData));
							fr->left = nullptr;
							fr->next = pg->freelist;
							pg->freelist->left = fr;
							#ifdef _DEBUG
							initbusyblocks--;
							#endif
							return true;
						}
					}
					// �������� ������ ������
					if (((BlockData*)p)->left != nullptr)
					{
						if ((((BlockData*)p)->left)->isBusy == false)
						{
							FreeList* fr = (FreeList*)((char*)(((BlockData*)p)->left) + sizeof(BlockData));
							//����������� ������ ������ �������� �������� � ��� �������
							fr->next = ((FreeList*)(((char*)p) + sizeof(BlockData)))->next;
							fr->left = ((FreeList*)(((char*)p) + sizeof(BlockData)))->left;

							//�������� �������� � ������ � ����� ������ ������� ������ ��������
							if(fr->left != nullptr)
							{
								(fr->left)->next = fr->next;
								if(((FreeList*)((char*)p + sizeof(BlockData)))->next!=nullptr)
									(((FreeList*)((char*)p + sizeof(BlockData)))->next)->left = fr;
							}
							else
							{
								pg->freelist = fr;
							}
							(((BlockData*)p)->left)->next = (((BlockData*)p)->next);
#ifdef _DEBUG
							initbusyblocks--;
#endif
						}
						else
						{
							FreeList* fr = (FreeList*)((char*)p + sizeof(BlockData));
							fr->left = nullptr;
							fr->next = pg->freelist;
							pg->freelist->left = fr;
							*pg->freelist->left = *fr;
							
#ifdef _DEBUG
							initfreeblocks++;
							initbusyblocks--;
#endif
						}
					}
					else
					{
						FreeList* fr = (FreeList*)((char*)p + sizeof(BlockData));
						fr->left = nullptr;
						fr->next = pg->freelist;
						pg->freelist->left = fr;
						*pg->freelist->left = *fr;
#ifdef _DEBUG
						initfreeblocks++;
						initbusyblocks--;
#endif
					}
					return true;
				}
			}
			if(pg->nextPage != nullptr) pg = pg->nextPage;
		} while (pg->nextPage != nullptr);
		return false;
	}

	void destroy() 
	{
		while (page->nextPage != nullptr)
		{
			VirtualFree((void*)(page), 0, MEM_RELEASE);
			page = page->nextPage;
		}
		VirtualFree((void*)(page), 0, MEM_RELEASE);
		page = nullptr;
	}

#ifdef _DEBUG
	void dumpStat() const 
	{
		std::setlocale(LC_ALL,"rus");
		std::cout << "\n\t���������� ������� � ��������� ������ Coalesce: " << initbusyblocks<<" , " << initfreeblocks << "\n\n\n";
		
	}
#endif
	
#ifdef _DEBUG
	void dumpBlocks() const{
		std::cout << "\n\tC����� ������, ����������� � �� � �� ������� Coalesce: ";
		std::cout << "\n\t|\t����� �����\t\t|�����/ �� �����|\t������\t|\n";
		Page* pg = page;
		while (pg != nullptr)
		{
			BlockData* b = (BlockData*)((char*)page + sizeof(Page));

			while (b != nullptr)
			{
				if(b->next==nullptr) std::cout << "\t|\t" << (void*)b << "\t|\t" << (bool)b->isBusy << "\t|\t" << (int)((char*)pg+numPage*size - (char*)b) << "\t|\n";
				else std::cout << "\t|\t" << (void*)b << "\t|\t" << (bool)b->isBusy << "\t|\t" << (int)(b->next - b) << "\t|\n";
				b = b->next;
			}
			pg = pg->nextPage;
		}
	}
#endif
};