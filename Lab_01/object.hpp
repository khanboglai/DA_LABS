#pragma once
#include <iostream>

class TObject {
    public:
        uint64_t key;
        std::string value;

        // constructors
        TObject();
        TObject(TObject&& other);
        ~TObject() noexcept;

        // operators
        TObject& operator=(const TObject& other);
};


TObject::TObject() {
    key = 0;
    value = "";
}

TObject::TObject(TObject&& other) {
    key = other.key;
    value = other.value;

    other.key = 0;
    other.value = "";
}

TObject::~TObject() noexcept {
    key = 0;
    value = " ";
}

TObject& TObject::operator=(const TObject& other) {
    key = other.key;
    value = other.value;
    return *this;
}
