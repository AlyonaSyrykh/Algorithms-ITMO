#include "DynamicArray.h" 
int main()
{
    Array<int> a;
    for (int i = 0; i < 10; ++i)
        a.insert(i + 1);
    a.insert(5, 1000);
    std::cout << "\n";
    a.insert(8, 1000);
    std::cout << "\n";
    a.insert(10, 1000);
    for (int i = 0; i < a.size(); ++i)
        std::cout << "a[" << i << "]= " << a[i] << "\n";
    a[7]++;
    std::cout << a[7] << "\n";
    a[9] *= 3;
    std::cout << a[9] << "\n";
    for (int i = 0; i < a.size(); ++i)
        std::cout << "a[" << i << "]= " << a[i] << "\n";
    Array<int>b = a;
    for (int i = 0; i < b.size(); ++i)
        std::cout << "b[" << i << "]= " << b[i] << "\n";
    a.remove(9);
    for (int i = 0; i < a.size(); ++i)
        std::cout << "a[" << i << "]= " << a[i] << "\n";
    for (auto it = a.reverseiterator();it.hasNext();it.next())
    {
        std::cout << it.get() << std::endl;
    }
    Array<int> d(18);
    std::cout << d.capaCity();
    return 0;
};