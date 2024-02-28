#pragma once
#include <stddef.h>


template<class T>
class TSimpleVector {
    private:
        T* data;
        size_t size;
        size_t cap;
    public:
        TSimpleVector();
        TSimpleVector(const size_t & n);
        TSimpleVector(const size_t & n, const T & value);
        ~TSimpleVector();

        void PushBack(const T & value);
        void PopBack();
        size_t Size();
        void Clear();
        void Print();

        T& operator[](const size_t & i);
        TSimpleVector& operator=(const T & value);
};


template<class T>
TSimpleVector<T>::TSimpleVector() {
    cap = 0;
    size = 0;
    data = nullptr;
}


template<class T>
TSimpleVector<T>::TSimpleVector(const size_t & n) {
    size = n;
    cap = n * 2;
    data = new T[cap];
}

template<class T>
TSimpleVector<T>::TSimpleVector(const size_t & n, const T & value) {
    size = n;
    cap = n * 2;
    data = new T[cap];
    for (size_t i = 0; i < size; i++) {
        data[i] = value;
    }
}

template<class T>
TSimpleVector<T>::~TSimpleVector() {
    size = 0;
    delete[] data;
}


template<class T>
void TSimpleVector<T>::PushBack(const T & value) {
    if (data == nullptr) {
        cap = 1;
        data = new T[cap];
    } else if (size == cap) {
        cap *= 2;
        T * tmp = new T[cap];

        for (size_t i = 0; i < size; i++) {
            tmp[i] = data[i];
        }

        delete[] data;
        data = tmp;
    }

    data[size] = value;
    size++;
}


template<class T>
void TSimpleVector<T>::PopBack() {
    if (data == nullptr) {
        return;
    } else if ((cap / (size - 1)) >= 2) {
        cap /= 2;
        T* tmp = new T[cap];
        for (size_t i = 0; i < size; i++) {
            tmp[i] = data[i];
        }

        delete[] data;
        data = tmp;
    }

    size--;
}


template<class T>
size_t TSimpleVector<T>::Size() {
    return this->size;
}


template<class T>
void TSimpleVector<T>::Clear() {
    size = 0;
    cap = 0;
    delete[] data;
    data = nullptr;
}


template<class T>
void TSimpleVector<T>::Print() {
    for (size_t i = 0; i < size; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}


template<class T>
T& TSimpleVector<T>::operator[](const size_t & i) {
    if (i < 0 || i >= size) {
        throw std::out_of_range("Invalid index");
    }

    return data[i];
}

template<class T>
TSimpleVector<T>& TSimpleVector<T>::operator=(const T & value) {
    int i;
    if (i < 0 || i >= size) {
        throw std::out_of_range("Inv idx");
    }

    data[i] = value;
    return this;
}
