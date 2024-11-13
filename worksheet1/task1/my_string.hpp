#ifndef MY_STRING_HPP
#define MY_STRING_HPP
// (header guards and endif help to prevent multiple inclusions of the same file)

#include <iostream>
#include <cstring> 

class my_string {
    private:
        char* data;  // pointer to data
        
    public:
    
    // default constructor
    my_string();
    
    // parameterised constructor that takes a C-style string
    // takes a const char* and creates a copy of it
    my_string(const char* str);
    
    // copy constructor
    my_string(const my_string& s);
    
    // assignment operator
    my_string& operator=(const my_string& s);
    
    // destructor
    ~my_string();
    
    // method to get a character at a given index at i
    char getChar(const int& i) const;

    // method to set a character at a given index at i to c
    void setChar(const int& i, const char& c);
    
    // just print
    void print() const;

};

#endif

