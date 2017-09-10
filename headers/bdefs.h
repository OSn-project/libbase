#ifndef __BDEFS_H__
#define __BDEFS_H__

#ifdef __linux__

#include <stdint.h>

/* fixed-size integer types */
typedef	int8_t					int8;
typedef uint8_t					uint8;
typedef	int16_t					int16;
typedef uint16_t				uint16;
typedef	int32_t					int32;
typedef uint32_t				uint32;
typedef	int64_t					int64;
typedef uint64_t				uint64;

/* descriptive types */
typedef int32					status_t;		// The status of a function call. Returns values from OSn errors.h
typedef int64					bigtime_t;
typedef int64					nanotime_t;		// Time but in nanoseconds
typedef uint32					type_code;
typedef uint32					perform_code;

#endif


#endif
