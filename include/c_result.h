#ifndef C_RESULT_H
#define C_RESULT_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Compiler-specific attributes for warning about unused results
#if defined(__GNUC__) || defined(__clang__)
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define WARN_UNUSED_RESULT
#endif

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

// Type tags for the value union
typedef enum {
    RESULT_TYPE_NONE, // Error result
    RESULT_TYPE_POINTER, // Generic pointer
    RESULT_TYPE_STRING, // char* (special case for strings)
    RESULT_TYPE_INT, // int
    RESULT_TYPE_LONG, // long
    RESULT_TYPE_FLOAT, // float
    RESULT_TYPE_DOUBLE, // double
    RESULT_TYPE_BOOL, // bool
    RESULT_TYPE_CHAR, // char
    RESULT_TYPE_SHORT, // short
    RESULT_TYPE_UINT, // unsigned int
    RESULT_TYPE_ULONG, // unsigned long
    RESULT_TYPE_SIZE_T, // size_t
    RESULT_TYPE_INT8, // int8_t
    RESULT_TYPE_UINT8, // uint8_t
    RESULT_TYPE_INT16, // int16_t
    RESULT_TYPE_UINT16, // uint16_t
    RESULT_TYPE_INT32, // int32_t
    RESULT_TYPE_UINT32, // uint32_t
    RESULT_TYPE_INT64, // int64_t
    RESULT_TYPE_UINT64, // uint64_t
    RESULT_TYPE_CUSTOM // For user-defined types
} ResultValueType;

// Custom type destructor function signature
typedef void (*CustomDestructor)(void *);

typedef struct {
    ResultCode code;
    ResultValueType value_type;
    bool owns_memory; // Indicates if Result owns allocated memory

    union {
        // Error data
        char *message;

        // Success values
        union {
            void *pointer;
            char *string;
            int int_val;
            long long_val;
            float float_val;
            double double_val;
            bool bool_val;
            char char_val;
            short short_val;
            unsigned int uint_val;
            unsigned long ulong_val;
            size_t size_val;
            int8_t int8_val;
            uint8_t uint8_val;
            int16_t int16_val;
            uint16_t uint16_val;
            int32_t int32_val;
            uint32_t uint32_val;
            int64_t int64_val;
            uint64_t uint64_val;

            // Custom type data
            struct {
                void *data;
                CustomDestructor destructor;
            } custom;
        } value;
    } data;
} Result;

// Error creation
WARN_UNUSED_RESULT static inline Result make_error(const ResultCode code, const char *message) {
    Result res = {0}; // Zero-initialize the entire struct
    res.code = code;
    res.value_type = RESULT_TYPE_NONE;
    res.owns_memory = true; // Error messages are always owned by the Result

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

// Success creation functions for different types
WARN_UNUSED_RESULT static inline Result make_success_ptr(void *value, const bool ownership) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_POINTER;
    res.owns_memory = ownership;
    res.data.value.pointer = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_string(char *value, const bool ownership) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_STRING;
    res.owns_memory = ownership;
    res.data.value.string = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_int(const int value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_INT;
    res.owns_memory = false;
    res.data.value.int_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_long(const long value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_LONG;
    res.owns_memory = false;
    res.data.value.long_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_float(const float value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_FLOAT;
    res.owns_memory = false;
    res.data.value.float_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_double(const double value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_DOUBLE;
    res.owns_memory = false;
    res.data.value.double_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_bool(const bool value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_BOOL;
    res.owns_memory = false;
    res.data.value.bool_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_char(const char value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_CHAR;
    res.owns_memory = false;
    res.data.value.char_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_short(const short value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_SHORT;
    res.owns_memory = false;
    res.data.value.short_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_uint(const unsigned int value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_UINT;
    res.owns_memory = false;
    res.data.value.uint_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_ulong(const unsigned long value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_ULONG;
    res.owns_memory = false;
    res.data.value.ulong_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_size_t(const size_t value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_SIZE_T;
    res.owns_memory = false;
    res.data.value.size_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_int8(const int8_t value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_INT8;
    res.owns_memory = false;
    res.data.value.int8_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_uint8(const uint8_t value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_UINT8;
    res.owns_memory = false;
    res.data.value.uint8_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_int16(const int16_t value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_INT16;
    res.owns_memory = false;
    res.data.value.int16_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_uint16(const uint16_t value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_UINT16;
    res.owns_memory = false;
    res.data.value.uint16_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_int32(const int32_t value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_INT32;
    res.owns_memory = false;
    res.data.value.int32_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_uint32(const uint32_t value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_UINT32;
    res.owns_memory = false;
    res.data.value.uint32_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_int64(const int64_t value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_INT64;
    res.owns_memory = false;
    res.data.value.int64_val = value;
    return res;
}

WARN_UNUSED_RESULT static inline Result make_success_uint64(const uint64_t value) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_UINT64;
    res.owns_memory = false;
    res.data.value.uint64_val = value;
    return res;
}

// Custom type creation
WARN_UNUSED_RESULT static inline Result make_success_custom(void *data, const CustomDestructor destructor,
                                                            const bool owns_memory) {
    Result res = {0};
    res.code = SUCCESS;
    res.value_type = RESULT_TYPE_CUSTOM;
    res.owns_memory = owns_memory;
    res.data.value.custom.data = data;
    res.data.value.custom.destructor = destructor;
    return res;
}

static inline bool is_result_success(const Result res) {
    return res.code == SUCCESS;
}

// Free result resources with ownership checking
// Free result resources
static inline void free_result(Result *result) {
    if (!result) return;

    if (is_result_success(*result) && result->owns_memory) {
        switch (result->value_type) {
            case RESULT_TYPE_POINTER:
                free(result->data.value.pointer);
                break;
            case RESULT_TYPE_STRING:
                free(result->data.value.string);
                break;
            case RESULT_TYPE_CUSTOM:
                if (result->data.value.custom.destructor) {
                    result->data.value.custom.destructor(result->data.value.custom.data);
                } else {
                    free(result->data.value.custom.data);
                }
                break;
            default:
                break;
        }
    } else if (result->owns_memory && result->data.message) {
        free(result->data.message);
    }

    result->code = ERR_INVALID_STATE;
    result->owns_memory = false;
}

// Type-safe access macros
#ifdef DEBUG
#include <stdio.h>
#define ACCESS_RESULT(result, type_enum, member, default_val) \
    (is_result_success(result) && (result).value_type == type_enum ? \
    (result).data.value.member : \
    (fprintf(stderr, "Error: Type mismatch at %s:%d (expected %d, got %d)\n", \
           __FILE__, __LINE__, type_enum, (result).value_type), (typeof((result).data.value.member))default_val))

#define access_result_ptr(result, type) ((type *)ACCESS_RESULT(result, RESULT_TYPE_POINTER, pointer, NULL))
#define access_result_string(result) ((char*)ACCESS_RESULT(result, RESULT_TYPE_STRING, string, (char*)NULL))
#define access_result_int(result) ACCESS_RESULT(result, RESULT_TYPE_INT, int_val, 0)
#define access_result_long(result) ACCESS_RESULT(result, RESULT_TYPE_LONG, long_val, 0L)
#define access_result_float(result) ACCESS_RESULT(result, RESULT_TYPE_FLOAT, float_val, 0.0f)
#define access_result_double(result) ACCESS_RESULT(result, RESULT_TYPE_DOUBLE, double_val, 0.0)
#define access_result_bool(result) ACCESS_RESULT(result, RESULT_TYPE_BOOL, bool_val, false)
#define access_result_char(result) ACCESS_RESULT(result, RESULT_TYPE_CHAR, char_val, '\0')
#define access_result_short(result) ACCESS_RESULT(result, RESULT_TYPE_SHORT, short_val, 0)
#define access_result_uint(result) ACCESS_RESULT(result, RESULT_TYPE_UINT, uint_val, 0U)
#define access_result_ulong(result) ACCESS_RESULT(result, RESULT_TYPE_ULONG, ulong_val, 0UL)
#define access_result_size_t(result) ACCESS_RESULT(result, RESULT_TYPE_SIZE_T, size_val, (size_t)0)
#define access_result_int8(result) ACCESS_RESULT(result, RESULT_TYPE_INT8, int8_val, (int8_t)0)
#define access_result_uint8(result) ACCESS_RESULT(result, RESULT_TYPE_UINT8, uint8_val, (uint8_t)0)
#define access_result_int16(result) ACCESS_RESULT(result, RESULT_TYPE_INT16, int16_val, (int16_t)0)
#define access_result_uint16(result) ACCESS_RESULT(result, RESULT_TYPE_UINT16, uint16_val, (uint16_t)0)
#define access_result_int32(result) ACCESS_RESULT(result, RESULT_TYPE_INT32, int32_val, (int32_t)0)
#define access_result_uint32(result) ACCESS_RESULT(result, RESULT_TYPE_UINT32, uint32_val, (uint32_t)0)
#define access_result_int64(result) ACCESS_RESULT(result, RESULT_TYPE_INT64, int64_val, (int64_t)0)
#define access_result_uint64(result) ACCESS_RESULT(result, RESULT_TYPE_UINT64, uint64_val, (uint64_t)0)
#define access_result_custom(result) ACCESS_RESULT(result, RESULT_TYPE_CUSTOM, custom.data, NULL)
#else
#define ACCESS_RESULT(result, type_enum, member, default_val) \
    (is_result_success(result) && (result).value_type == type_enum ? (result).data.value.member : default_val)

// Same macros without debug checks but with basic type validation
#define access_result_ptr(result, type) ((type *)(is_result_success(result) ? (result).data.value.pointer : NULL))
#define access_result_string(result) (is_result_success(result) ? (result).data.value.string : NULL)
#define access_result_int(result) (is_result_success(result) ? (result).data.value.int_val : 0)
#define access_result_long(result) (is_result_success(result) ? (result).data.value.long_val : 0L)
#define access_result_float(result) (is_result_success(result) ? (result).data.value.float_val : 0.0f)
#define access_result_double(result) (is_result_success(result) ? (result).data.value.double_val : 0.0)
#define access_result_bool(result) (is_result_success(result) ? (result).data.value.bool_val : false)
#define access_result_char(result) (is_result_success(result) ? (result).data.value.char_val : '\0')
#define access_result_short(result) (is_result_success(result) ? (result).data.value.short_val : 0)
#define access_result_uint(result) (is_result_success(result) ? (result).data.value.uint_val : 0U)
#define access_result_ulong(result) (is_result_success(result) ? (result).data.value.ulong_val : 0UL)
#define access_result_size_t(result) (is_result_success(result) ? (result).data.value.size_val : (size_t)0)
#define access_result_int8(result) (is_result_success(result) ? (result).data.value.int8_val : (int8_t)0)
#define access_result_uint8(result) (is_result_success(result) ? (result).data.value.uint8_val : (uint8_t)0)
#define access_result_int16(result) (is_result_success(result) ? (result).data.value.int16_val : (int16_t)0)
#define access_result_uint16(result) (is_result_success(result) ? (result).data.value.uint16_val : (uint16_t)0)
#define access_result_int32(result) (is_result_success(result) ? (result).data.value.int32_val : (int32_t)0)
#define access_result_uint32(result) (is_result_success(result) ? (result).data.value.uint32_val : (uint32_t)0)
#define access_result_int64(result) (is_result_success(result) ? (result).data.value.int64_val : (int64_t)0)
#define access_result_uint64(result) (is_result_success(result) ? (result).data.value.uint64_val : (uint64_t)0)
#define access_result_custom(result) (is_result_success(result) ? (result).data.value.custom.data : NULL)
#endif

// Helper function to get error message
static inline const char *result_get_error_message(const Result *result) {
    if (is_result_success(*result) || result->data.message == NULL) {
        return "No error message";
    }
    return result->data.message;
}

// Helper function to check if result owns memory
static inline bool result_owns_memory(const Result *result) {
    return result->owns_memory;
}

// Helper function to transfer memory ownership from result
static inline void result_transfer_ownership(Result *result, bool transfer_ownership) {
    result->owns_memory = transfer_ownership;
}

#endif // C_RESULT_H
