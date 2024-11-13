// my_string2.h
#ifndef MY_STRING2_H
#define MY_STRING2_H

#include <iostream>
#include <cstring> 

class my_string {
    private:
        char* data;  // pointer to the actual string data
        int* refs; // reference counter 

    public:
        my_string();
        my_string(const char* str);
        my_string(const my_string& s);
        my_string& operator=(const my_string& s);
        ~my_string();

        char getChar(const int& i) const;
        void setChar(const int& i, const char& c);
        void print() const;

};

#endif