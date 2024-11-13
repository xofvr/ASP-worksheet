#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

class point {
public:
    // Constructor
    point() {
        std::cout << "point object created!" << std::endl;
    }

    // Destructor
    ~point() {
        std::cout << "point object destroyed!" << std::endl;
    }

    // Simple method to test functionality
    void show() const {
        std::cout << "This is a point object." << std::endl;
    }
};

#endif