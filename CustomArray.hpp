#pragma once
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class CustomArray
{
private:
    T *data;
    int capacity;
    int currentSize;

    void resize()
    {
        capacity *= 2;
        T *newData = new T[capacity];
        for (int i = 0; i < currentSize; i++)
            newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    CustomArray() : capacity(5), currentSize(0)
    {
        data = new T[capacity];
    }

    ~CustomArray()
    {
        delete[] data;
    }

    CustomArray(const CustomArray &other) : capacity(other.capacity), currentSize(other.currentSize)
    {
        data = new T[capacity];
        for (int i = 0; i < currentSize; i++)
            data[i] = other.data[i];
    }

    CustomArray &operator=(const CustomArray &other)
    {
        if (this == &other)
            return *this;
        delete[] data;
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new T[capacity];
        for (int i = 0; i < currentSize; i++)
            data[i] = other.data[i];
        return *this;
    }

    void add(T item)
    {
        if (currentSize == capacity)
            resize();
        data[currentSize++] = item;
    }

    T get(int index) const
    {
        if (index < 0 || index >= currentSize)
            throw out_of_range("Index out of bounds.");
        return data[index];
    }

    int size() const
    {
        return currentSize;
    }
};