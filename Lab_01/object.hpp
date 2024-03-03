#pragma once
#include <iostream>

class TObject {
    public:
        uint64_t key;
        std::string value;

        // constructors
        TObject();
        TObject(const uint64_t& c_key, const std::string& c_value);
        TObject(const TObject& other);
        ~TObject() noexcept;

        // operators
        TObject& operator=(const TObject& other);
        friend std::ostream& operator<<(std::ostream& os, const TObject & other) {
            os << other.key << "\t" << other.value;
            return os;
        }
};


TObject::TObject() {
    key = 0;
    value = "";
}

TObject::TObject(const TObject& other) {
    key = other.key;
    value = other.value;

}


TObject::TObject(const uint64_t& c_key, const std::string& c_value) {
    key = c_key;
    value = c_value;
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
