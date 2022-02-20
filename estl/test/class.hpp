#pragma once
#include <utility.hpp>


namespace estd::test {
class A {
public:
    A() : value(nullptr) {}
    
    A(int v) : value(new int(v)) {}

    A(const A& rhs) : value(rhs.value ? new int(*rhs.value) : nullptr) {}

    A(A&& rhs) : A() { 
        rhs.swap(*this);
    }
    ~A() {
        if (value) delete value;
    }

    void swap(A& rhs) {
        estd::swap(value, rhs.value);
    }

    A& operator=(A rhs) {
        rhs.swap(*this);
        return *this;
    }

public:
    int* value;
};
}   // namespace Test