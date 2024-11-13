#include "my_string2.hpp"

// Default constructor which includes initialising the reference counter to 1
my_string::my_string() : data(nullptr), refs(new int(1)){
}

my_string::my_string(const char* str) {
    data = new char[strlen(str) + 1]; 
    strcpy(data, str);  
    refs = new int(1); // initialising the ref counter
    // 1 because null is still a valid state
}


my_string::my_string(const my_string& s) {
    data = s.data;  // shallow copy: both objects share the same memory
    refs = s.refs; // incrementing the refs counter
    (*refs)++;
}

// assignment operator
my_string& my_string::operator=(const my_string& s) {
    if (this == &s) return *this; 

    // if the current object is the last one to reference the data, delete the data
    if (--(*refs) == 0) {
        delete[] data;
        delete refs; 
    }

    data = s.data;  // shallow copy
    refs = s.refs; // incrementing the refs counter
    (*refs)++;

    return *this;
}

my_string::~my_string() {
    if (--(*refs) == 0) {
        delete[] data;
        delete refs;
    }
}

char my_string::getChar(const int& i) const {
    // Implementation here
    return data[i];
}

void my_string::setChar(const int& i, const char& c) {
    // Implementation here
    data[i] = c;
}

void my_string::print() const {
    // Implementation here
    std::cout << data << " [" << *refs << "]" << std::endl;
}
