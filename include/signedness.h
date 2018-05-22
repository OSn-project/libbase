/*!These functions are used to convert between	*
 * signed and unsigned integers.				*/

#ifndef __SIGNEDNESS_H__
#define __SIGNEDNESS_H__

#include <base.h>

#ifdef __cplusplus
extern "C" {
#endif

inline uint16 b_stou16(uint16 signed_int);		//! Signed to unsigned (16-bit)
inline uint16 b_utos16(uint16 unsigned_int);	//! Unsigned to signed (16-bit)
inline uint32 b_stou32(uint32 signed_int);		//! Signed to unsigned (32-bit)
inline uint32 b_utos32(uint32 unsigned_int);	//! Unsigned to signed (32-bit)
inline uint64 b_stou64(uint64 signed_int);		//! Signed to unsigned (64-bit)
inline uint64 b_utos64(uint64 unsigned_int);	//! Unsigned to signed (64-bit)

#endif
#endif
