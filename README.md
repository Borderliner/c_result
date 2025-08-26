# c_result
This is my attempt at a lightweight, header-only, cross-platform, C99-compatible C library for elegant error handling using a `Result` type pattern, remotely similar to Rust's `Result` or Haskell's `Either`. This library provides a clean way to handle both success and error states without complex exception mechanisms.

*⚠️ Attention: Use with care and review the code before you use it in production.*
## Features
- **📄 Header-only**: Just include `c_result.h` in your project
- **✅ Type-safe error handling (manual)**: Clear separation between success and error states
- **🧐 Rich error codes**: Predefined common error codes with meaningful descriptions
- **♟️ Custom error support**: Easy extension for application-specific error codes
- **🛟 Memory safe (manual)**: Proper memory management for error messages and values
- **🔬 Debug support**: Optional runtime checks for safe value access in debug builds

## Usage
### Basic Example
```c
#include "c_result.h"

Result divide_numbers(int a, int b) {
    if (b == 0) {
        return make_error(ERR_INVALID_OPERATION, "Division by zero");
    }
    int* result = malloc(sizeof(int));
    *result = a / b;
    return make_success(result);
}

int main() {
    Result res = divide_numbers(10, 2);
    if (is_success(res)) {
        int* value = ACCESS_RESULT_VALUE(res, int);
        printf("Result: %d\n", *value);
        free_result(res, int);
    } else {
        fprintf(stderr, "Error: %s (code: %d)\n", res.data.message ? res.data.message : "Unknown error", res.code);
    }
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
    // File processing logic here
    // ...
    return make_success_string(file_contents);
}
```
## API Reference
### Core Types
- `ResultCode`: Integer type for error codes
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
- `make_error(code, message)`: Create an error result with message
- `make_success(value)`: Create a success result with a value
- `make_success_string(value)`: Convenience macro for string values
- `make_success_int(value)`: Convenience macro for integer values
- `is_success(result)`: Check if a result represents success
- `free_result(res, type)`: Properly free a result's resources
- `ACCESS_RESULT_VALUE(res, type)`: Safely access the value of a successful result

## Memory Management
The library handles memory allocation for error messages automatically. When using `make_success()` with allocated memory, you must use `free_result()` to properly clean up:

```c
Result res = some_operation();
// ... use the result ...
free_result(res, YourType); // Properly cleans up based on success/error state
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
In debug mode, `ACCESS_RESULT_VALUE` will check if you're trying to access a value from an error result and print a warning message.

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
