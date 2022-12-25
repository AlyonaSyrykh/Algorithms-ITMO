#include "pch.h"
#include "../Sort/DynamicArray.h"
#include "../Sort/QuickSort.h"
TEST(TestInsertion, TestArray) {
	Array<int> a;
	for (int k = 30;k >= 0;k--)
		a.insert(rand()%1000);
	insertionsort(&a[0], &a[a.size()-1], [](int a, int b) { return a < b; });
	Array<int> b = a;
	std::sort(&b[0], &b[b.size()], [](int a, int b) { return a < b; });
	for (int i = 0; i < a.size();i++) {
		EXPECT_EQ(b[i], a[i]);
	}
}

TEST(TestSort, TestVectorOne) {
	std::vector<int> b;
	for (int k = 0;k < 10;k++)
		b.push_back(1);
	for (int k = 10;k < 40;k++)
		b.push_back(2);
	std::vector<int> c = b;
	sort(b.begin(), b.end(), [](int a, int b) { return a > b; });
	std::sort(c.begin(), c.end(), [](int a, int b) { return a > b;});
	for (int i = 0; i < b.size();i++)
		EXPECT_EQ(c[i], b[i]);
}

TEST(TestSort, TestVector) {
	std::vector<int> b;
	for (int k = 0;k < 100;k++)
		b.push_back(k);
	std::vector<int> c = b;
	sort(b.begin(), b.end(), [](int a, int b) { return a > b; });
	std::sort(c.begin(), c.end(), [](int a, int b) { return a > b;});
	for (int i = 0; i < b.size();i++)
		EXPECT_EQ(c[i], b[i]);
}
TEST(TestSort, TestArray) 
{
	Array<int> a;
	for (int k = 0;k < 100;k++)
		a.insert(rand()%1000);
	Array<int> b = a;
	sort(&a[0], &a[a.size()-1], [](int a, int b) { return a > b; });
	std::sort(&b[0], &b[b.size()], [](int a, int b) { return a > b; });
	for (int i = 0; i < a.size();i++) {
		EXPECT_EQ(b[i], a[i]);
	}
}

TEST(TestSort, TestChar)
{
	char a[10];
	for (int k = 0;k < 10;k++)
		a[k]=k;
	sort(&a[0], &a[9], [](char a, char b) { return a < b; });
	for (int i = 0; i < 9;i++) {
		EXPECT_EQ((char)i, a[i]);
	}
}

TEST(TestSort, TestString)
{
	Array<std::string> a;
	for (int k = 9;k >= 0;k--)
		a.insert(std::to_string(k)+"@@@@@@@@@@@@@@@@@@@@@@");
	sort(&a[0], &a[a.size()-1], [](std::string a, std::string b) { return a < b; });
	for (int i = 0; i < 10;i++) {
		EXPECT_EQ(std::to_string(i) + "@@@@@@@@@@@@@@@@@@@@@@", a[i]);
	}
}