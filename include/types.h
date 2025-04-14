
#ifndef __CJLF_TYPES
#define __CJLF_TYPES

#define Nil 0

#define BEFORE __attribute__((__constructor__))
#define AFTER __attribute__((__destructor__))

#define OMENAMESH_API __attribute__((visibility("default"))) extern
#define ARRAY_SIZE(arr) ((sizeof(arr)) / (sizeof(*arr)))
#define CJLF_NULL (void *)0
#define CJLF_FAILED_MEM_MAP (void *)-1

/*mostly local & APIs that might change in the future*/
#define OMENAMESH_API_T __attribute__((visibility("hidden")))
#define VOLATILE OMENAMESH_API_T

typedef unsigned char u8__CJLF;
typedef unsigned short u16__CJLF;
typedef unsigned u32__CJLF;
typedef unsigned long long u64__CJLF;

/*! its really painful casting this type, it doesnt even make sense to typedef
 * it*/
typedef signed char i8__CJLF;
typedef short i16__CJLF;
typedef int i32__CJLF;
typedef long long i64__CJLF;

typedef void __CJLF_GENERICS;

/*common*/
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>  /*virtual mem*/
#include <pthread.h> /*posix threads*/
#include <stdbool.h> /*bool*/
#include <stdint.h>  /*types*/
#include <stdio.h>   /*I/O*/
#include <stdlib.h>
#include <string.h> /*string*/
#include <unistd.h>

#endif /*! __CJLF_TYPES*/