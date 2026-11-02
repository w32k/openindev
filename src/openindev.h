#ifndef OPENINDEV_H_INCLUDED
#define OPENINDEV_H_INCLUDED




#ifdef _WIN32
#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define ARR_LEN(x) (sizeof(x) / sizeof(x[0]))


















#endif
