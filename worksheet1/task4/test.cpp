#include "ref_count.hpp"
#include "test.hpp"

int main() {
    {
        std::cout << "Creating first point object (p1)..." << std::endl;
        refcount<point> p1(new point());
        std::cout << "Reference count for p1: " << p1.get_ref_count() << std::endl;
        p1->show();

        {
            std::cout << "\nCreating second reference to the same point object (p2 = p1)..." << std::endl;
            refcount<point> p2 = p1;  // p2 now shares the same point as p1
            std::cout << "Reference count for p1: " << p1.get_ref_count() << std::endl;
            std::cout << "Reference count for p2: " << p2.get_ref_count() << std::endl;
            p2->show();
        }  // p2 goes out of scope, reference count decreases

        std::cout << "\nBack to first reference (p1)..." << std::endl;
        std::cout << "Reference count for p1: " << p1.get_ref_count() << std::endl;
        p1->show();
    }  // p1 goes out of scope, reference count reaches 0, object destroyed

    std::cout << "\nNow testing with basic data types like int...\n" << std::endl;
    {
        refcount<int> num1(new int(42));  // Creates a reference-counted integer
        std::cout << "Reference count for num1: " << num1.get_ref_count() << std::endl;
        std::cout << "Value of num1: " << *num1 << std::endl;

        {
            std::cout << "\nCreating second reference to the same int (num2 = num1)..." << std::endl;
            refcount<int> num2 = num1;  // Shares the same integer
            std::cout << "Reference count for num1: " << num1.get_ref_count() << std::endl;
            std::cout << "Reference count for num2: " << num2.get_ref_count() << std::endl;
            std::cout << "Value of num2 (shared with num1): " << *num2 << std::endl;
        }  // num2 goes out of scope, but num1 still holds the reference

        std::cout << "\nValue of num1 after num2 goes out of scope: " << *num1 << std::endl;
        std::cout << "Reference count for num1: " << num1.get_ref_count() << std::endl;
    }  // num1 goes out of scope, reference count reaches 0, int is deleted

    return 0;
}
