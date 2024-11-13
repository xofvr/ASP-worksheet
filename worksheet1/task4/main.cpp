#include "my_string4.hpp"

int main() {
    // Create an initial my_string instance
    my_string str1("Hello, world!");
    str1.print();  // Expected output: "Hello, world! [1]"

    // Copy constructor: Create a new my_string from str1
    my_string str2 = str1;
    str1.print();  // Expected output: "Hello, world! [2]"
    str2.print();  // Expected output: "Hello, world! [2]"

    // Modify str2 and check that the reference count stays the same
    str2.setChar(7, 'W');  // Change "world" to "World" in str2
    str1.print();          // Expected output: "Hello, World! [2]"
    str2.print();          // Expected output: "Hello, World! [2]"

    // Assignment operator: Assign str2 to a new my_string, str3
    my_string str3;
    str3 = str2;
    str1.print();  // Expected output: "Hello, World! [3]"
    str2.print();  // Expected output: "Hello, World! [3]"
    str3.print();  // Expected output: "Hello, World! [3]"

    // Modify str3 and observe changes in str1 and str2
    str3.setChar(0, 'h');  // Change "Hello" to "hello"
    str1.print();          // Expected output: "hello, World! [3]"
    str2.print();          // Expected output: "hello, World! [3]"
    str3.print();          // Expected output: "hello, World! [3]"

    return 0;
}
