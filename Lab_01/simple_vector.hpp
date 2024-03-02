#pragma once
#include <stddef.h>


template<class T>
class TSimpleVector {
    private:
        T* buffer;
        size_t size;
        size_t cap;
    public:

        // constructors
        TSimpleVector();
        TSimpleVector(const size_t & n);
        TSimpleVector(const size_t & n, const T & value);
        TSimpleVector(TSimpleVector<T>&& other); // move constructor
        ~TSimpleVector();

        // methods
        void PushBack(const T & value);
        void PopBack();
        size_t Size() const; // getter
        void Print();

        // operators
        T& operator[](const size_t & i);
        TSimpleVector& operator=(TSimpleVector<T>&& other);
        
        friend std::ostream& operator<<(std::ostream& os, const TSimpleVector<T>& other) {
            for (int i = 0; i < other.size; i++) {
                os << other.buffer[i];
            }

            return os;
        }
};


template<class T>
TSimpleVector<T>::TSimpleVector() {
    cap = 0;
    size = 0;
    buffer = nullptr;
}


template<class T>
TSimpleVector<T>::TSimpleVector(const size_t & n) {
    size = n;
    cap = n;
    buffer = new T[cap];
}

template<class T>
TSimpleVector<T>::TSimpleVector(const size_t & n, const T & value) {
    size = n;
    cap = n;
    buffer = new T[cap];
    for (size_t i = 0; i < size; i++) {
        buffer[i] = value;
    }
}


template<class T>
TSimpleVector<T>::TSimpleVector(TSimpleVector&& other) {
    delete[] buffer;
    cap = other.cap;
    size = other.size;

    buffer = new T[cap];
    
    for (size_t i = 0; i < other.size; i++) {
        buffer[i] = other.buffer[i];
    }

    other.cap = 0;
    other.size = 0;
    delete[] buffer; // leak was here
    other.buffer = nullptr;
}


template<class T>
TSimpleVector<T>::~TSimpleVector() {
    size = 0;
    cap = 0;
    delete[] buffer;
}


template<class T>
void TSimpleVector<T>::PushBack(const T & value) {
    if (buffer == nullptr) {
        cap = 1;
        buffer = new T[cap];
    }
    
    if (size == cap) {
        cap *= 2;
        T * tmp = new T[cap];

        for (size_t i = 0; i < size; i++) {
            tmp[i] = buffer[i];
        }

        delete[] buffer;
        buffer = tmp;
    }

    buffer[size] = value;
    size++;
}


template<class T>
void TSimpleVector<T>::PopBack() {
    if (buffer == nullptr) {
        return;
    } else if ((cap / (size - 1)) >= 2) {
        cap /= 2;
        T* tmp = new T[cap];
        for (size_t i = 0; i < size; i++) {
            tmp[i] = buffer[i];
        }

        delete[] buffer;
        buffer = tmp;
    }

    size--;
}


template<class T>
size_t TSimpleVector<T>::Size() const {
    return this->size;
}


template<class T>
void TSimpleVector<T>::Print() {
    for (size_t i = 0; i < size; i++) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
}


template<class T>
T& TSimpleVector<T>::operator[](const size_t & i) {
    if (i < 0 || i >= size) {
        throw std::out_of_range("Invalid index");
    }

    return buffer[i];
}


template<class T>
TSimpleVector<T>& TSimpleVector<T>::operator=(TSimpleVector<T>&& other) {
    delete[] buffer;
    size = other.size;
    cap = other.cap;
    buffer = new T[cap];

    for (size_t i = 0; i < size; i++) {
        buffer[i] = other.buffer[i];
    }

    other.cap = 0;
    other.size = 0;
    
    // leak was here
    delete[] other.buffer;
    other.buffer = nullptr;

    return *this;
}
