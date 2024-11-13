#include "my_string2.hpp"

int main() {
    my_string s("Hello world!");
    s.print();
    {
        my_string t = s; // calls the copy constructor
        s.print();
        t.print(); // we copied s, t should be the same
        std::cout << s.getChar(1) << std::endl; // Should print the character at index 1
        s.print(); // should print the string as no modifications were made
        t.print(); // again, same thing should be printed
    }
    s.setChar(1, 'E'); // will change the character at index 1 to E
    s.print(); // will print the modified string
    // t.print(); // would throw an error as t is now out of scope

    return 0;
}
