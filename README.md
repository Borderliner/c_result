# c_result
This is my attempt at a lightweight, header-only, cross-platform, C99-compatible library for elegant error handling using a Result type pattern, inspired by Rust's Result and Haskell's Either. This library provides a clean way to handle both success and error states with enhanced type safety and memory management.

*⚠️ Attention: Use with care and review the code before you use it in production.*
## Features
- **📄 Header-only**: Just include [`c_result.h`](include/c_result.h) in your project
- **✅ Enhanced type safety**: Strong typing for different value types with runtime type checking
- **🧐 Rich error codes**: Predefined common error codes with meaningful descriptions
- **♟️ Custom type support**: Support for custom types with custom destructors
- **🛟 Memory ownership control**: Explicit memory ownership management
- **🔬 Debug support**: Runtime type checking and validation in debug builds
- **📏 Standard types**: Support for all standard integer types, floats, and custom types
- **⚠️ Compiler warnings**: `warn_unused_result` attribute for major compilers (GCC/Clang)

## Usage
### Basic Example
```c
#include "c_result.h"

Result divide_numbers(int a, int b) {
    if (b == 0) {
        return make_error(ERR_INVALID_OPERATION, "Division by zero");
    }
    return make_success_int(a / b);
}

int main() {
    Result res = divide_numbers(10, 2);
    if (is_result_success(res)) {
        int value = access_result_int(res);
        printf("Result: %d\n", value);
    } else {
        fprintf(stderr, "Error: %s (code: %d)\n", 
                result_get_error_message(&res), res.code);
    }
    free_result(&res);
    return 0;
}
```

### File Processing Example
For a more complete test version of this example, check [`test/c_result.c`](test/c_result.c) file in the repo.
```c
Result read_file_contents(const char* filename) {
    if (!filename) {
        return make_error(ERR_INVALID_INPUT, "Filename cannot be NULL");
    }
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        return make_error(ERR_FILE_NOT_FOUND, "File not found");
    }
    
    // Read file contents
    char* buffer = malloc(1024);
    if (!buffer) {
        fclose(file);
        return make_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory");
    }
    
    // File processing logic here
    // ...
    
    fclose(file);
    return make_success_string(buffer, true); // Result takes ownership of buffer
}
```
### Custom Type Example
```c
typedef struct {
    int id;
    char* name;
} Person;

void person_destructor(void* data) {
    Person* person = (Person*)data;
    free(person->name);
    free(person);
}

Result create_person(int id, const char* name) {
    Person* person = malloc(sizeof(Person));
    if (!person) {
        return make_error(ERR_OUT_OF_MEMORY, "Failed to allocate person");
    }
    
    person->id = id;
    person->name = malloc(strlen(name) + 1);
    if (!person->name) {
        free(person);
        return make_error(ERR_OUT_OF_MEMORY, "Failed to allocate name");
    }
    strcpy(person->name, name);
    
    return make_success_custom(person, person_destructor, true);
}
```
## API Reference
### Core Types
- `ResultCode`: Integer type for error codes
- `ResultValueType`: Enumeration of supported value types
- `Result`: Structure containing either a success value or error information

### Predefined Error Codes
The library provides numerous predefined error codes:

| Error Code | Description |
|------------|-------------|
| `SUCCESS` (0) | Operation completed successfully |
| `ERR_INVALID_INPUT` (-1) | Invalid input parameters |
| `ERR_OUT_OF_MEMORY` (-2) | Memory allocation failure |
| `ERR_FILE_IO` (-3) | File I/O operation failed |
| `ERR_NULL_POINTER` (-4) | Unexpected null pointer |
| ... | ... |

Check out [`c_result.h`](include/c_result.h) defines at the top of the file, or use your own codes. Error codes of 0 to -999 are reserved.

Application-specific error codes should start at `APP_RESULT_CODE_START` (-1000).
### Key Functions and Macros
#### Result Creation
- `make_error(code, message)`: Create an error result with message
- `make_success_*()`: Functions for creating success results of various types
  * `make_success_ptr(value, ownership)`: For generic pointers
  * `make_success_string(value, ownership)`: For string values
  * `make_success_int(value)`: For integer values
  * `make_success_custom(data, destructor, ownership)`: For custom types

#### Result Inspection
- `is_result_success(result)`: Check if a result represents success
- `result_get_error_message(result)`: Get error message
- `result_owns_memory(result)`: Check if result owns memory
- `result_transfer_ownership(result, transfer)`: Transfer memory ownership

#### Result Value Access
- `access_result_*()`: Macros for type-safe value access
  * `access_result_ptr(result, type)`: Access pointer values
  * `access_result_int(result)`: Access integer values
  * `access_result_string(result)`: Access string values
  * `access_result_custom(result)`: Access custom values

#### Memory Management
- `free_result(&result)`: Properly free a result's resources

## Memory Management
The library provides explicit control over memory ownership. When creating a success result, you can specify whether the Result should take ownership of the memory:

```c
// Example 1: Result doesn't take ownership
int value = 42;
Result res = make_success_int(value);

// Example 2: Result takes ownership of allocated memory
char* buffer = malloc(1024);
Result res = make_success_string(buffer, true);

// Example 3: Custom type with destructor
Person* person = create_person();
Result res = make_success_custom(person, person_destructor, true);
```
Always use free_result() to properly clean up:
```c
Result res = some_operation();
// ... use the result ...
free_result(&res); // Properly cleans up based on success/error state and ownership
```

## Custom Error Codes
Define application-specific error codes starting from `APP_RESULT_CODE_START`:

```c
#define ERR_MY_APP_ERROR (APP_RESULT_CODE_START - 0)
#define ERR_MY_OTHER_ERROR (APP_RESULT_CODE_START - 1)
```

## Debug Mode
Compile with `-DDEBUG` to enable additional safety checks:
```shell
gcc -DDEBUG -o myapp myapp.c
```
In debug mode, value access macros will check:

- If you're trying to access a value from an error result
- If the value type matches the accessor type
- Will print warning messages on type mismatches

## Integration
Since this is a header-only library, simply copy [`c_result.h`](include/c_result.h) to your project and include it:
```c
#include "c_result.h"
```
## License
This library is provided under BSD-3-Clause license. Feel free to use and modify according to your needs.

## Contributing
Contributions are welcome! Please ensure any changes maintain backward compatibility and include appropriate tests.

# Author
Made with ❤️ by Mohammadreza Hajianpour
