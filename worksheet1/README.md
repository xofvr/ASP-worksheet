# asp_worksheet

# Task 1 

This implements a custom `my_string` class with:
- Copy constructor and assignment operator sharing the same memory (shallow copy).
- The destructor will remain empty for now.

### Implementation Details

The `my_string` class consists of the following methods:

1. **Constructors**:
   - **Default Constructor**: Initialises the `data` pointer to `nullptr`. 
   - **Parameterised Constructor**: Takes a C-style string (`const char*`), allocates memory dynamically for it, and copies the content. This ensures that `my_string` objects can hold strings of any length.
   - **Copy Constructor**: Shares the same memory between instances instead of performing a deep copy, implementing a **shallow copy**. 

2. **Assignment Operator** (`operator=`):
   - Ensures self-assignment safety by checking if the current instance is the same as the assigned instance.
   - Deletes the previously allocated memory before assigning the `data` pointer to the new instance's memory, effectively making it a shallow copy as well.

3. **Destructor** (`~my_string()`):
   - Left empty. 

4. **Methods**:
   - **`getChar(int i)`**: Returns the character at index `i` in the string.
   - **`setChar(int i, char c)`**: Sets the character at index `i` to `c`.
   - **`print()`**: Prints the current string.

### Code Structure

- **my_string.cpp**: Has the method definitions for constructors, assignment operator, and character manipulation methods.
- **my_string.hpp**: Declares the `my_string` class and its methods, with header guards to prevent multiple inclusions.
- **main.cpp**: Has the main function.


### Expected Output

Compiling and running the provided `main.cpp` with `Hello world!` will produce the following output:


![Task 1 Output](./img/Task1.png)

# Task 2 - Reference Counting Implementation

This is an extension of the basic string class to include automatic reference counting. It has a shared counter that tracks how many string objects are referencing the same data.

The class manages two key pointers:
- A pointer to character data (data) storing the actual string
   - Initialises as a nullptr
- A pointer to an integer (refs) storing the reference count
   - Initialises as 1 

When a new string is created, it starts with a reference count of 1. During copy operations or assignments, instead of creating a new copy of the data, the new object shares the existing data and increments the reference counter. When an object is destroyed or reassigned, the counter decrements. Only when the counter reaches zero (indicating no more objects are using the data) is the memory actually freed.

``` 
Initial state:
s1("Hello world!") → data="Hello world!", refs=1

After copy:
s1("Hello world!") ↘
             data="Hello world!", refs=2
s2("Hello world!") ↗

After s2 destruction:
s1("Hello world!") → data="Hello world!", refs=1

After s1 destruction:
data and refs are deleted
```

## Program Output
When run, the program does the reference counting by showing:
- Initial string creation with reference count [1]
- Copying operations that increase the count to [2]
- Shared data modifications affecting all references
- Reference count decreasing when objects go out of scope

![Task 2 Output](./img/Task2.png)


# Task 3 - Destructor Implementation
The destructor frees allocated memory only when refs reaches zero.
![Task 3 Output](./img/Task3.png)

# Task 4 - Smart Pointer 
The ref_count file is a template class that is loosely based on a smart pointer.

When an object is created, a reference count is initialised to 1. If a new reference to the object is made (using copy constructor or assignment), the reference count is incremented.
If the object is assigned to another, the reference count of the old object is decremented. When the count reaches zero, the old object is deleted. The new object’s reference count is incremented.
When an object is destroyed, the reference count is decremented. If the count reaches zero, the object and its reference count are deleted. 

![Task 4 Output](./img/Task4.png)

