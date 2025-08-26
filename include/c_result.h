#ifndef C_RESULT_LIBRARY_H
#define C_RESULT_LIBRARY_H

#include <string.h>
#include <stdlib.h>

typedef int ResultCode;

// Core Error codes (reserved range: 0-(-999))
#define SUCCESS 0
#define ERR_INVALID_INPUT (-1)
#define ERR_OUT_OF_MEMORY (-2)
#define ERR_FILE_IO (-3)
#define ERR_NULL_POINTER (-4)
#define ERR_BUFFER_OVERFLOW (-5)
#define ERR_FILE_NOT_FOUND (-6)
#define ERR_PERMISSION_DENIED (-7)
#define ERR_NOT_IMPLEMENTED (-8)
#define ERR_TIMEOUT (-9)
#define ERR_RESOURCE_BUSY (-10)
#define ERR_INVALID_STATE (-11)
#define ERR_OUT_OF_RANGE (-12)
#define ERR_BAD_FORMAT (-13)
#define ERR_NO_SPACE (-14)
#define ERR_ALREADY_EXISTS (-15)
#define ERR_NOT_FOUND (-16)
#define ERR_INVALID_OPERATION (-17)
#define ERR_NETWORK_ERROR (-18)
#define ERR_CONNECTION_FAILED (-19)
#define ERR_INTERRUPTED (-20)
#define ERR_DEVICE_ERROR (-21)
#define ERR_PROTOCOL_ERROR (-22)
#define ERR_INVALID_CONFIG (-23)
#define ERR_LOCK_FAILED (-24)
#define ERR_TOO_MANY_OPEN_FILES (-25)
#define ERR_INVALID_PATH (-26)
#define ERR_READ_ONLY (-27)
#define ERR_NOT_A_DIRECTORY (-28)
#define ERR_DIRECTORY_NOT_EMPTY (-29)
#define ERR_BROKEN_PIPE (-30)
#define ERR_MATH_DOMAIN (-31)
#define ERR_ILLEGAL_SEQUENCE (-32)
#define ERR_UNKNOWN (-33)

// App-defined result codes should start at -1000
#define APP_RESULT_CODE_START (-1000)

typedef struct {
    ResultCode code;

    union {
        char *message;
        void *value;
    } data;
} Result;

static inline Result make_error(const ResultCode code, const char *message) {
    Result res = {.code = code, .data.message = NULL};
    if (message) {
        res.data.message = malloc(strlen(message) + 1);
        if (res.data.message) {
            strcpy(res.data.message, message);
        } else {
            res.code = ERR_OUT_OF_MEMORY;
        }
    }
    return res;
}

static inline Result make_success(void *value) {
    const Result res = { .code = SUCCESS, .data.value = value };
    return res;
}

// #define make_success(value) ((Result){ .code = SUCCESS, .data.value = (value) })
#define make_success_string(value) make_success((char *)(value))
#define make_success_int(value) make_success((void *)(long)(value))
#define is_success(result) ((result).code == SUCCESS)
#define free_result(result, type) \
    do { \
    if (is_success(result)) { \
    free((type *)result.data.value); \
    } else if (result.data.message) { \
    free(result.data.message); \
    } \
    } while (0)

#ifdef DEBUG
#define ACCESS_RESULT_VALUE(res, type) \
    (IS_SUCCESS(res) ? (type *)res.data.value : (fprintf(stderr, "Error: Attempt to access value on error result at %s:%d\n", __FILE__, __LINE__), (type *)NULL))
#else
#define ACCESS_RESULT_VALUE(res, type) ((type *)res.data.value)
#endif

#endif // C_RESULT_LIBRARY_H
