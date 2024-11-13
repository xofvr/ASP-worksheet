#include "my_string.hpp"

// default constructor
//  will crash if the string is empty!
my_string::my_string() : data(nullptr){
}

// c style string constructor
my_string::my_string(const char* str) {
    data = new char[strlen(str) + 1];  // allocating space for the string and +1 for null terminator
    strcpy(data, str); // copying the string into the memory 

}

// copy constructor
my_string::my_string(const my_string& s) {
    // this shares the same memory, so instead of deep copy we do a shallow copy
    data = s.data; 
}

// assignment operator
my_string& my_string::operator=(const my_string& s) {
    // handles self assignment
    if (this == &s) return *this; 
    delete[] data;  // delete old memory
    data = s.data;  // shallow copy
    return *this;
}

my_string::~my_string() {
    // empty as its a different task
}

// method to get a character at a given index
char my_string::getChar(const int& i) const {
    // i will be the index of the character so return the character at that index
    return data[i];
}

// method to set a character at a given index
void my_string::setChar(const int& i, const char& c) {
    // sets the caracter at index i to c
    data[i] = c;
}

// Print the string 
void my_string::print() const {

    std::cout << data << std::endl;
}

