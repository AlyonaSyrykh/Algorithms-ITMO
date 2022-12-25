#include <iostream>

template<typename T>
class Array final
{
private:

    T* arr;
    int SIZE;
    int capacity;
    int reSIZE;

public:

    Array()
    {
        SIZE = 0;
        capacity = 8;
        reSIZE = 2;
        arr = (T*)malloc(capacity * sizeof(T));
    };

    Array(int capacity)
    {
        this->capacity = capacity;
        SIZE = 0;
        reSIZE = 2;
        arr = (T*)malloc(this->capacity * sizeof(T));
    };

    ~Array()
    {
        for (int i = 0; i < SIZE; ++i) arr[i].~T();
        free(arr);
    };

    //увеличение размера массива
    void resize(int index, const T& value)
    {
        capacity *= reSIZE;
        T* arr1 = (T*)malloc(capacity * sizeof(T));
        for (int i = 0; i < index; i++) new(arr1 + i) T(std::move(arr[i]));
        new (arr1 + index) T(std::move(value));
        for (int i = index+1; i < SIZE; i++) new(arr1 + i) T(std::move(arr[i-1]));
        for (int i = 0; i < SIZE; i++) arr[i].~T();
        free(arr);
        arr = arr1;
    };

    //конструктор копирования
    Array(const Array& other)
    {
        arr = (T*)malloc(other.capacity * sizeof(T));
        for (int i = 0;i < other.SIZE;i++) new(arr+i) T(other.arr[i]);
        capacity = other.capacity;
        SIZE = other.SIZE;
        reSIZE = other.reSIZE;
    };

    //конструктор перемещения
    Array(Array && other) noexcept 
    {
        arr = other.arr;
        SIZE = other.SIZE;
        reSIZE = other.reSIZE;
        capacity = other.capacity;
        other.arr = nullptr;
    }

    //добавление элемента в конец массива
    int insert(const T& value)
    {
        if (capacity == SIZE) resize(SIZE, value);
        else new(arr + SIZE) T(value);
        return (SIZE++);
    }

    //добавление элемента в указанное место массива
    int insert(int index, const T& value)
    {
        if (capacity == SIZE) resize(index, value);
        else
        {
            new (arr + SIZE) T(std::move(arr[SIZE - 1]));
            for (int i = SIZE-1; i > index; i--) arr[i + 1] = arr[i];
            arr[index + 1] = arr[index];
            arr[index] = value;
        }
        SIZE++;
        return SIZE;
    }

    //удаление выбранного элемента массива
    void remove(int index)
    {
        if (index>=0 && index<SIZE) 
        {
            for (int i = index; i < SIZE - 1;i++) 
            {
                arr[i].~T();
                arr[i] = T(std::move(arr[i + 1]));
            }
            arr[SIZE-1].~T();
            SIZE--;
        }
    }

    //перегрузка индексирования
    const T& operator[](int index) const { if(index<SIZE) return *(arr + index); };

    T& operator[](int index) { return *(arr + index); };

    //перегрузка копирующего присваивания
    /*Array& operator =(Array other)
    {
        if (this != &other) 
        {
            std::swap(arr, other.arr);
            std::swap(capacity, other.capacity);
            std::swap(SIZE, other.SIZE);
            std::swap(reSIZE, other.reSIZE);
            return *this;
        }
        return *this;
    };*/

    //перегрузка копирующего присваивания
    Array& operator =(Array& other)
    {
        if (this != &other)
        {
            Array<T> arr_t(other);
            std::swap(arr, arr_t.arr);
            std::swap(capacity, arr_t.capacity);
            std::swap(SIZE, arr_t.SIZE);
            std::swap(reSIZE, arr_t.reSIZE);
            return *this;
        }
        return *this;
    };

    //перегрузка присваивания перемещением
    Array& operator =(Array&& other) noexcept
    {
        for (int i = 0;i < SIZE; i++) arr[i].~T();
        free(arr);
        this->arr = other.arr;
        this->SIZE = other.SIZE;
        this->capacity = other.capacity;
        this->reSIZE = other.reSIZE;
        other.arr = nullptr;
        return *this;
    };
    
    T& operator ++(int value) { return this++; };
    T& operator *=(int value) { return this *= value; };
    T& operator -=(int value) { return this -= value; };
    T& operator +=(int value) { return this += value; };

    //получение текущего размера массива
    int size() const
    {
        return SIZE;
    };

    //получение текущего capacity
    int capaCity() const 
    {
        return capacity;
    };

    class Iterator
    {

    public:

        Iterator(Array<T>* a, bool reverse)
        {
            this->reverse = reverse;
            if (reverse) element = a->arr+a->SIZE-1;
            else element = a->arr;
            root = a->arr;
            size = a->SIZE;
        };

        //получает значение массива в текущей позиции итератора
        const T& get() const
        {
            return *element;
        };

        //устанавливает значение в текущей позиции итератора
        void set(const T& value)
        {
            *element = value;
        };

        //перемещает текущую позицию итератора на следующий элемент
        void next()
        {
            if (reverse) element--;
            else element++;
        };

        //возвращает true, если итератор может перейти к следующему элементу, или false в противном случае.
        bool hasNext() const
        {
            if (reverse)
            {
                if (element == root-1) return false;
                else return true;
            }
            else if (element == root+size) return false;
            return true;
        };

        T& operator+ (int value) { return *(element + value); }
        T& operator- (int value) { return *(element - value); }
        T& operator++ (int) { if (reverse) return *element--; else return *element++; }
        T& operator-- (int) { if (reverse) return *element++; else return *element--; }
        T& operator++ () { if (reverse) return *--element; else return *++element; }
        T& operator-- () { if (reverse) return *++element; else return *--element; }
        bool operator!= (const Iterator& it) { return element != it.element; }
        bool operator== (const Iterator& it) { return element == it.element; }
        T& operator* () { return *element; }
        
    private:
        T* element;
        T* root;
        size_t size;
        bool reverse;
    };

    Iterator iterator()
    {
        return Iterator(this, false);
    };

    Iterator reverseiterator()
    {
        return Iterator(this, true);
    };
};


