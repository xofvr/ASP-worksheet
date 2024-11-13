#include "my_string3.hpp"

my_string::my_string() : data(nullptr), refs(new int(1)){
}

my_string::my_string(const char* str) {
    data = new char[strlen(str) + 1];  // +1 for null terminator
    strcpy(data, str);  // Copy the input string into the newly allocated memory
    refs = new int(1); // Initialising the ref counter
}

// Copy constructor
my_string::my_string(const my_string& s) {
    data = s.data;  // shallow copy: both objects share the same memory
    refs = s.refs; // incrementing the refs counter
    (*refs)++;
}

// Assignment operator
my_string& my_string::operator=(const my_string& s) {
    if (this == &s) return *this;  // Handle self-assignment   

    if (--(*refs) == 0) {
        delete[] data;
        delete refs;
    }

    data = s.data;  // shallow copy
    refs = s.refs; // incrementing the refs counter
    (*refs)++;

    return *this;

}

// Destructor
my_string::~my_string() {
    if (--(*refs) == 0) {
        std::cout << "Deleting and freeing memory for: " << data << std::endl;
        delete[] data;
        delete refs;
    }

}

// Method to get a character at a given index
char my_string::getChar(const int& i) const {
    // Implementation here
    return data[i];
}

// Method to set a character at a given index
void my_string::setChar(const int& i, const char& c) {
    // Implementation here
    data[i] = c;
}

// Print the string
void my_string::print() const {
    // Implementation here
    std::cout << data << " [" << *refs << "]" << std::endl;
}

