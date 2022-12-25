#pragma once
#include <iostream>
#include "DynamicArray.h"
#include <chrono> 
#include <vector>

template<typename T, typename Compare>
void sort(T* first, T* last, Compare comp)
{
    if (last - first < 16) insertionsort(first, last, comp);
    else quicksort(first, last, comp);
}

template<typename T, typename Compare>
void quicksort(T* first, T* last, Compare comp)
{
    while (first < last) 
    {
        //выбор опорного элемента
        T* middle = first + (last - first) / 2;
        T pivot = *middle;
        if (*middle > *last && *last > *first || *last < *first && *middle < *last) pivot = *last;
        if (*middle > *first && *first > *last || *first < *last && *middle < *first) pivot = *first;
        T* left_ = first;
        T* right_ = last;
        while (true)
        {
            while (comp(*left_, pivot)) left_++;
            while (comp(pivot, *right_)) right_--;
            if (left_ >= right_) break;
            std::swap(*left_, *right_);
            left_++;
            right_--;
        }
       
        if (last - left_ > left_ - first)
        {
            quicksort(first, right_, comp);
            first = right_+1;
        }
        else
        {
            quicksort(right_+1, last, comp);
            last = right_;
        }
    }
}

template<typename T, typename Compare>
void insertionsort(T* first, T* last, Compare comp)
{
    for (int i = 1;i < last - first + 1; i++)
        for (int j = i;j > 0 && comp(*(first + j), *(first + j - 1)); j--)
            std::swap(*(first + j - 1), *(first + j));
}


   
    


