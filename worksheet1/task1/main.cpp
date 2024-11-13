#include "my_string.hpp"

int main() {
    my_string s("");
    s.print();
    {
        my_string t = s; // calls the copy const
        s.print(); // main thing 
        t.print(); // we copied s, t should be the same
        std::cout << s.getChar(1) << std::endl; // should print the character at index 1
        s.print(); // should print the string as no modifications were made
        t.print(); // again, same thing should be printed
    }
    s.setChar(1, 'E'); // change the character at index 1 to E
    s.print(); 
    //t.print(); // should throw an error as t is now out of scope
}