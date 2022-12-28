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
		//получение первой страницы и выделение пам€ти на нее
		page = (Page*)pBuf;// присваиваем указатель первой страницы на pBuf
		page->nextPage = nullptr;
		//описание дл€ блока
		BlockData* block = (BlockData*)((char*)page + sizeof(Page));
		block->next = nullptr;
		block->left = nullptr;
		block->isBusy = false;
		//описание дл€ фрилиста
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
			if (list != nullptr) //провер€ем, есть ли в списке свободных блоков блок нужного размера
			{
				BlockData* data = (BlockData*)((char*)list - sizeof(BlockData));
				if ((char*)data->next - (char*)data >= size)//поиск блока, в который можно поместить данные соответствующего размера
				{
					data->isBusy = true;
					if ((char*)data->next - (char*)data >= size + 512 + sizeof(BlockData)) //проверка на возможность разделени€ блока на несколько блоков
					{
						BlockData *bd = (BlockData*)(data + size);
						bd->isBusy = false;
						bd->left = data;
						bd->next = data->next;//создаем новый блок
						if(data->next != nullptr) (data->next)->left = bd;//указываем на новый блок прошлому следующему блоку как на предыдущий
						else data->next = bd; // указываем на новый блок как следующий в текущем
						//убираем из фрилиста этот блок и добавл€ем новый при возможности
						FreeList* fr = (FreeList*)((char*)data->next + sizeof(BlockData)); //указываем на новый блок данных
						if (list->left == nullptr)//если удал€ем голову
						{
							fr->left = nullptr;
							if (list->next == nullptr) fr->next = nullptr; //единственный ли это был элемент
							else
							{
								fr->next = list->next;
								(list->next)->left = fr;
							}
							//записываем в пам€ти указатель на след.свободный блок
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
					else //только один 
					{
						if (list->left == nullptr)//если удал€ем голову
						{
							if (list->next == nullptr) list = nullptr; //единственный ли это был элемент
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
				((BlockData*)p)->isBusy = false;//показываем, что блок теперь свободен
				if (((BlockData*)p)->next != nullptr || ((BlockData*)p)->left != nullptr) 
				{

					//проверка правого соседа
					if (((BlockData*)p)->next != nullptr) 
					{
						if ((((BlockData*)p)->next)->isBusy == false)
						{
							//переуказали хвосты правого элемента фрилиста в наш элемент
							((FreeList*)((char*)p + sizeof(BlockData)))->next = ((FreeList*)(((char*)((BlockData*)p)->next) + sizeof(BlockData)))->next;
							((FreeList*)((char*)p + sizeof(BlockData)))->left = ((FreeList*)(((char*)((BlockData*)p)->next) + sizeof(BlockData)))->left;
							if (((FreeList*)(((char*)p) + sizeof(BlockData)))->left == nullptr) 
							{
								pg->freelist = (FreeList*)(((char*)p) + sizeof(BlockData));
							}
							else 
								//обновили значени€ в правом и левом соседе бывшего правого элемента
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
					// проверка левого соседа
					if (((BlockData*)p)->left != nullptr)
					{
						if ((((BlockData*)p)->left)->isBusy == false)
						{
							FreeList* fr = (FreeList*)((char*)(((BlockData*)p)->left) + sizeof(BlockData));
							//переуказали хвосты левого элемента фрилиста в наш элемент
							fr->next = ((FreeList*)(((char*)p) + sizeof(BlockData)))->next;
							fr->left = ((FreeList*)(((char*)p) + sizeof(BlockData)))->left;

							//обновили значени€ в правом и левом соседе бывшего левого элемента
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
		std::cout << "\n\t оличество зан€тых и свободных блоков Coalesce: " << initbusyblocks<<" , " << initfreeblocks << "\n\n\n";
		
	}
#endif
	
#ifdef _DEBUG
	void dumpBlocks() const{
		std::cout << "\n\tCписок блоков, запрошенных у ќ— и их размеры Coalesce: ";
		std::cout << "\n\t|\tјдрес блока\t\t|«ан€т/ не зан€т|\t–азмер\t|\n";
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