#ifndef OPENINDEV_H_INCLUDED
#define OPENINDEV_H_INCLUDED







#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#define U64_PRINT "%lld"
#else
#define U64_PRINT "%ld"
#endif
#define VER "INDEV"
#define IMPL "MC 1.7.10"
#define DEBUG_FAIL(message, ...) printf("[FAIL] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_PASS(message, ...) printf("[PASS] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_INFO(message, ...) printf("[INFO] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_WARNING(message, ...) printf("[WARNING] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)

typedef uint32_t handle_t;
typedef float vec3_t[3];
typedef float vec4_t[4];
typedef vec4_t mat4_t[4];

#define ARR_LEN(x) (sizeof(x) / sizeof(x[0]))








#endif
