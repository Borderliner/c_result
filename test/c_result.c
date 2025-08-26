#include <stdio.h>
#include <stdlib.h>
#include "c_result.h"

#define ERR_APP_INVALID_BYTES_READ (APP_RESULT_CODE_START - 0)
Result process_file(const char* filename) {
    if (!filename || !filename[0]) {
        return make_error(ERR_INVALID_INPUT, "Invalid filename");
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        return make_error(ERR_FILE_IO, "Failed to open file");
    }

    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        return make_error(ERR_OUT_OF_MEMORY, "Failed to allocate memory");
    }

    const size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read != file_size) {
        free(buffer);
        fclose(file);
        char message[128];
        sprintf(message, "Invalid amount of read bytes. Expected: %ld bytes, read %zu\n", file_size, bytes_read);
        return make_error(ERR_APP_INVALID_BYTES_READ, message);
    }

    buffer[bytes_read] = '\0';
    fclose(file);
    return make_success_string(buffer, true);
}

int main() {
    Result res = process_file("test.txt");
    if (is_result_success(res)) {
        printf("Operation successful: %s\n", access_result_string(res));
        free_result(&res);
    } else {
        fprintf(stderr, "Operation failed: %s (code: %d)\n", result_get_error_message(&res), res.code);
        free_result(&res);
    }
    return 0;
}
