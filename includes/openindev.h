#ifndef OPENINDEV_H_INCLUDED
#define OPENINDEV_H_INCLUDED







#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#define DEBUG_FAIL(message, ...) printf("[FAIL] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_PASS(message, ...) printf("[PASS] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_INFO(message, ...) printf("[INFO] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG_WARNING(message, ...) printf("[WARNING] (%s:%d) " message, __FILE__, __LINE__, ##__VA_ARGS__)













#endif
