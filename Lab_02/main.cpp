#include "BT.hpp"


int main() {
    BT<int> t(2);
    t.Insert(3);
    t.Insert(1);
    t.Insert(2);
    t.Insert(0);
    t.Print();
}
