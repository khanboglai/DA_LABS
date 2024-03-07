#pragma once
#include <stddef.h>


template<class T>
class TSimpleVector {
    private:
        T* buffer;
        int size;
        int cap;
    public:

        // constructors
        TSimpleVector();
        TSimpleVector(const int & n);
        TSimpleVector(const int & n, const T & value);
        TSimpleVector(TSimpleVector<T>&& other); // move constructor
        ~TSimpleVector();

        // for sort benchmark c++
        T* Begin() {
            return buffer;
        }

        T* End() {
            return buffer + size;
        }

        // methods
        void Reserve(const int new_cap) {
            if (cap > new_cap) {
                return;
            }

            T* new_buffer = new T[new_cap];

            for (int i = 0; i < size; i++) {
                new_buffer[i] = std::move(buffer[i]);
            }

            delete[] buffer;
            buffer = new_buffer;
            cap = new_cap;
        } 


        void PushBack(const T & value);

        void PopBack();
        int Size() const; // getter

        // operators
        T& operator[](const int & i);
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
TSimpleVector<T>::TSimpleVector(const int & n) {
    size = n;
    cap = n;
    buffer = new T[cap];
}

template<class T>
TSimpleVector<T>::TSimpleVector(const int & n, const T & value) {
    size = n;
    cap = n;
    buffer = new T[cap];
    for (int i = 0; i < size; i++) {
        buffer[i] = std::forward<T>(value);
    }
}


template<class T>
TSimpleVector<T>::TSimpleVector(TSimpleVector&& other) {
    delete[] buffer;
    cap = other.cap;
    size = other.size;

    buffer = new T[cap];
    
    for (int i = 0; i < other.size; i++) {
        buffer[i] = std::forward<T>(other.buffer[i]);
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
        Reserve(100);
    }
    
    if (size == cap) {
        Reserve(size * 2);
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
        for (int i = 0; i < size; i++) {
            tmp[i] = buffer[i];
        }

        delete[] buffer;
        buffer = tmp;
    }

    size--;
}


template<class T>
int TSimpleVector<T>::Size() const {
    return this->size;
}


template<class T>
T& TSimpleVector<T>::operator[](const int & i) {
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

    for (int i = 0; i < size; i++) {
        buffer[i] = std::forward<T>(other.buffer[i]);
    }

    other.cap = 0;
    other.size = 0;
    
    // leak was here
    delete[] other.buffer;
    other.buffer = nullptr;

    return *this;
}
