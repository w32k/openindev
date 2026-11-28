#ifndef VERMILION_H_INCLUDED
#define VERMILION_H_INCLUDED







#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CLIENT_NAME "Vermilion"
#define VER "INDEV 0.1"
#define IMPL_VER "MC 1.7.10"

#define DEBUG_FAIL(message, ...) printf("[FAIL] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_PASS(message, ...) printf("[PASS] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_INFO(message, ...) printf("[INFO] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_WARNING(message, ...) printf("[WARNING] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)

typedef uint32_t shandle_t;
typedef float vec3_t[3];
typedef float vec4_t[4];
typedef vec4_t mat4_t[4];

#define ARR_LEN(x) (sizeof(x) / sizeof(x[0]))








#endif
