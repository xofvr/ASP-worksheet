#include "my_string4.hpp"

// Default constructor
my_string::my_string() : data(new char[1]) {
    data.get()[0] = '\0';
}

// Parameterized constructor
my_string::my_string(const char* str) : data(new char[std::strlen(str) + 1]) {
    std::strcpy(data.get(), str);
}

// Copy constructor
my_string::my_string(const my_string& s) : data(s.data) {}

// Assignment operator
my_string& my_string::operator=(const my_string& s) {
    if (this != &s) {
        data = s.data;
    }
    return *this;
}

// Destructor
my_string::~my_string() {}

// Method to get a character at a given index
char my_string::getChar(const int& i) const {
    return data[i];
}

// Method to set a character at a given index
void my_string::setChar(const int& i, const char& c) {
    data[i] = c;
}

// Print the string
void my_string::print() const {
    std::cout << data.get() << " [" << data.get_ref_count() << "]" << std::endl;
}

