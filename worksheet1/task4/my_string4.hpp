
#include <iostream>
#include <cstring>
#include "ref_count.hpp"

class my_string {
public:
    // Default constructor
    my_string();

    // Parameterized constructor that takes a C-style string
    my_string(const char* str);

    // Copy constructor
    my_string(const my_string& s);

    // Assignment operator
    my_string& operator=(const my_string& s);

    // Destructor
    ~my_string();

    // Method to get a character at a given index
    char getChar(const int& i) const;

    // Method to set a character at a given index
    void setChar(const int& i, const char& c);

    // Print the string
    void print() const;

private:
    refcount<char> data; // Pointer to the character array
};

