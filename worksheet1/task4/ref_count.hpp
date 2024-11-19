#include <iostream>
template<typename T>
class refcount {
    
public:
    // Constructor: Takes a pointer to the object we want to manage
    refcount(T* obj) : data(obj), refs(new int(1)) {}

    // Copy constructor: Shares the same object and increments the reference count
    refcount(const refcount& other) : data(other.data), refs(other.refs) {
        (*refs)++;
    }

    // Assignment operator: Handles self-assignment and reference counting
 refcount& operator=(const refcount& other) {
        if (this == &other) return *this; // Handle self-assignment

        // Decrement the old object's reference count and delete if no references remain
        if (--(*refs) == 0) {
            delete data;  // Corrected from delete[] data
            delete refs;
        }

        // Share the new object's data and reference count
        data = other.data;
        refs = other.refs;
        (*refs)++; // Increment the reference count

        return *this;
    }

    // Destructor: Decrements the reference count and deletes the object if no references remain
    ~refcount() {
        if (--(*refs) == 0) {
            delete data;  // Corrected from delete[] data
            delete refs;
        }
    }

    // Access the managed object
    T* get() const { return data; }

    // Dereference operator to access the object directly
    T& operator*() const { return *data; }

    // Member access operator
    T* operator->() const { return data; }

    // Subscript operator to access elements
    T& operator[](int index) const {
        return data[index];
    }

    // New method to get the reference count
    int get_ref_count() const { return *refs; }

private:
    T* data;   // Pointer to the object
    int* refs; // Pointer to the reference count
};