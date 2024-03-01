#pragma once
#include <iostream>

const int STRING_SIZE = 2048;

class TMyString {
    private:
        char* buffer;
        int size;

    public:
        TMyString() {
            size = 0;
            buffer = nullptr;
        }

        void PushBack(const char & s) {
            if (buffer == nullptr) {
                buffer = new char[STRING_SIZE];
            }

            buffer[size] = s;
            size++;
        }

        ~TMyString() {
            size = 0;
            delete[] buffer;
            buffer = nullptr;
        }

        friend std::ostream & operator<<(std::ostream& os, const TMyString & other_str) {
            for (int i = 0; i < other_str.size; i++) {
                os << other_str.buffer[i];
            }
            return os;
        }

};