#include <iostream>
#include "DynamicArray.h"
#include "QuickSort.h"
#include <chrono>

void timetest()
{
	for (int i = 1;i < 50;i++)
	{
		auto begin = std::chrono::steady_clock::now();
		for (int j = 0; j < 100000; j++)
		{
			Array<int> a;
			for (int k = 0;k < i;k++)
				a.insert(rand() % 1000);
			quicksort(&a[0], &a[a.size() - 1], [](int a, int b) { return a < b; });
		}
		auto end = std::chrono::steady_clock::now();
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
		std::cout << elapsed_ms.count() << "\n";
	}
	std::cout << "\n\nsortarray\n";
	for (int i = 1;i < 50;i++)
	{
		auto begin = std::chrono::steady_clock::now();
		for (int j = 0; j < 100000; j++)
		{
			Array<int> a;
			for (int k = i;k > 0;k--)
				a.insert(k);
			insertionsort(&a[0], &a[a.size() - 1], [](int a, int b) { return a < b; });
		}
		auto end = std::chrono::steady_clock::now();
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
		std::cout << elapsed_ms.count() << "\n";
	}
}

int main()
{
	/*Array<int> a;
	for (int k = 0;k < 50;k++)
	{
		a.insert(rand() % 1000);
		std::cout << a[k] << "\n";
	}
	quicksort(&a[0], &a[a.size() - 1], [](int a, int b) { return a < b; });
	for (int i = 0;i < 50;i++) {
		std::cout << a[i] << "\n";
	}*/
	timetest();
	return 0;
}


