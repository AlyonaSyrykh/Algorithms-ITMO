#include "pch.h"
#include "../DynamicArray/DynamicArray.h"

TEST(TestInsert, TestInsertIntRoot) {
	Array<int> a;
	a.insert(1);
	ASSERT_EQ(a[0], 1);
}
TEST(TestInsert, TestInsertDoubleRoot) {
	Array<double> a;
	a.insert(1.1);
	ASSERT_EQ(a[0], 1.1);
}
TEST(TestInsert, TestInsertCharRoot) {
	Array<char> a;
	a.insert('o');
	ASSERT_EQ(a[0], 'o');
}
TEST(TestInsert, TestInsertIntMiddle) {
	Array<int> a;
	for (int i = 0; i < 12;i++) {
		a.insert(1 + i);
	}
	ASSERT_EQ(a[11], 12);
}

TEST(TestInsert, TestInsertDoubleMiddle) {
	Array<double> a;
	for (int i = 0; i < 12;i++) {
		a.insert(1.1 + i);
	}
	ASSERT_EQ(a[11],12.1);
}
TEST(TestInsert, TestInsertCharMiddle) {
	Array<char> a;
	a.insert('o');
	a.insert('q');
	ASSERT_EQ(a[1], 'q');
}
TEST(TestInsertIndex, TestInsertIntRootIndex) {
	Array<int> a;
	for (int i = 0;i < 9;i++) {
		a.insert(i+1);
	}
	a.insert(0, 1000);
	ASSERT_EQ(a[0], 1000);
}

TEST(TestInsertIndex, TestInsertDoubleRootIndex) {
	Array<double> a;
	for (int i = 0; i < 4;i++) {
		a.insert(1.1 + i);
	}
	a.insert(0, 1.1);
	ASSERT_EQ(a[0],1.1);
}
TEST(TestInsertIndex, TestInsertCharRootIndex) {
	Array<char> a;
	a.insert('o');
	a.insert('q');
	a.insert(0, 'w');
	ASSERT_EQ(a[0], 'w');
}
TEST(TestSizeGet, TestSizeGetInitialize) 
{
	Array<char> a;
	ASSERT_EQ(a.size(), 0);
}

TEST(TestSizeGet, TestSizeGetInsert) {
	Array<char> a;
	a.insert('o');
	a.insert('q');
	a.insert(0, 'w');
	ASSERT_EQ(a.size(), 3);
}

TEST(TestSizeGet, TestSizeGetRemove) {
	Array<char> a;
	a.insert('o');
	a.insert('q');
	a.insert(0, 'w');
	a.remove(1);
	ASSERT_EQ(a.size(), 2);
}

TEST(TestDelete, TestDeleteIntRoot) {
	Array<int> a;
	for (int i = 0; i < 7;i++) {
		a.insert(1 + i);
	}
	a.remove(0);
	ASSERT_EQ(a[0], 2);
}
TEST(TestDelete, TestDeleteDoubleRoot) {
	Array<double> a;
	for (int i = 0; i < 4;i++) {
		a.insert(1.1 + i);
	}
	a.remove(0);
	ASSERT_EQ(a[0], 2.1);
}
TEST(TestDelete, TestDeleteCharRoot) {
	Array<char> a;
	a.insert('o');
	a.insert('q');
	a.insert('y');
	a.remove(0);
	ASSERT_EQ(a[0], 'q');
}
TEST(TestDelete, TestDeleteIntMiddle) {
	Array<int> a;
	for (int i = 0; i < 7;i++) a.insert(1 + i);
	a.remove(2);
	ASSERT_EQ(a[2], 4);
}

TEST(TestDelete, TestDeleteDoubleMiddle) {
	Array<double> a;
	for (int i = 0; i < 7;i++) a.insert(1.1 + i);
	a.remove(2);
	ASSERT_EQ(a[5], 7.1);
}

TEST(TestDelete, TestDeleteCharMiddle) {
	Array<char> a;
	a.insert('o');
	a.insert('q');
	a.insert('i');
	a.remove(-3);
	ASSERT_EQ(a[1], 'q');
}

TEST(TestDelete, TestDeleteFull) {
	Array<char> a;
	a.insert('o');
	a.insert('q');
	a.insert('i');
	a.remove(0);
	a.remove(0);
	a.remove(0);
	a.remove(0);
	a.insert('p');
	ASSERT_TRUE(a[0]=='p');
}

TEST(TestIterator, TestIteratorIncrement) 
{
	Array<int> a;
	for (int i = 0; i < 7;i++) a.insert(1 + i);
	int i = 0;
	for (auto it = a.iterator();it.hasNext();it++) 
	{
		ASSERT_TRUE(a[i]==it.get());
		i++;
	}
}

TEST(TestIterator, TestReverseIteratorIncrement)
{
	Array<int> a;
	for (int i = 0; i < 7;i++) {
		a.insert(1 + i);
	}
	int i = a.size()-1;
	for (auto it = a.reverseiterator();it.hasNext();it++)
	{
		ASSERT_TRUE(a[i]==it.get());
		i--;
	}
}

TEST(CopyTest, CopyConstructor)
{
	Array<int> a;
	for (int i = 0; i < 9; ++i) a.insert(i);
	Array<int> b(a);
	for (int i = 0; i < a.size(); ++i)
	{
		ASSERT_EQ(a[i], b[i]);
	}
	ASSERT_EQ(a.size(), b.size());
	ASSERT_EQ(a.capaCity(), b.capaCity());
}

TEST(CopyTest, MoveConstructor)
{
	Array<int> a;
	for (int i = 0; i < 9; ++i) a.insert(i);
	Array<int> b = std::move(a);
	EXPECT_EQ(9, b.size());
	EXPECT_EQ(a.capaCity(), b.capaCity());
	for (int i = 0; i < 9; ++i) {
		EXPECT_EQ(i, b[i]);
	}
}

TEST(CopyTest, CopyOperator)
{
	Array<int> a(4);
	for (int i = 0; i < 7; ++i) a.insert(i);
	Array<int> b;
	b.insert(1);
	b = a;
	EXPECT_EQ(7, b.size());
	EXPECT_EQ(8, b.capaCity());
	for (int i = 0; i < a.size(); ++i) {
		EXPECT_EQ(b[i], a[i]);
	}
}

TEST(CopyTest, MoveOperator)
{
	int capacity = 10;
	int size = 5;
	Array<int> a(capacity);
	for (int i = 0; i < size; ++i) a.insert(i);
	Array<int> b{ 5 };
	b = std::move(a);
	EXPECT_EQ(size, b.size());
	EXPECT_EQ(capacity, b.capaCity());
	for (int i = 0; i < b.size(); ++i) {
		EXPECT_EQ(b[i], i);
	}
}

TEST(StringTest, StringInitialization) {
	Array<std::string> b(10);
	ASSERT_EQ(b.size(), 0);
	ASSERT_EQ(b.capaCity(), 10);
}

TEST(StringTest, StringInitializationDefault) {
	Array<std::string> b;
	ASSERT_EQ(b.size(), 0);
	ASSERT_EQ(b.capaCity(), 8);
}

TEST(TestInsert, InsertString)
{
	Array<std::string> a(10);
	for (int i = 0; i < 15; ++i) a.insert("o");
	ASSERT_EQ(a.size(), 15);
	std::string value = "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
	a.insert(5, value);
	for (int i = 0; i < 5; ++i) {
		EXPECT_EQ("o", a[i]);
	}
	EXPECT_EQ(value, a[5]);
	for (int i = 6; i < a.size() - 1; ++i) {
		EXPECT_EQ("o", a[i]);
	}
}

TEST(TestDelete, TestDeleteString)
{
	int size = 7;
	Array<std::string> a(size);
	for (int i = 0; i < size; ++i) a.insert("o");
	a.remove(5);
	for (int i = 0; i < size - 1; ++i) {
		if (i < 5) {
			EXPECT_EQ("o", a[i]);
		}
		else if (i > 5) {
			EXPECT_EQ("o", a[i - 1]);
		}
	}
}


