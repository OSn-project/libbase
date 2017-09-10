/*!These functions convert integers between		*
 * system-endianness and little/big-endianness.	*/

#ifndef __BSTRING_H__
#define __BSTRING_H__

#include <base.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CPU_LITTLE_ENDIAN
//#define CPU_BIG_ENDIAN

/* little-endian */
inline uint16 b_stol16(uint16 system);		//! System-endian to little-endian (16-bit)
inline uint16 b_ltos16(uint16 little);		//! Little-endian to system-endian (16-bit)
inline uint32 b_stol32(uint32 system);		//! System-endian to little-endian (32-bit)
inline uint32 b_ltos32(uint32 little);		//! Little-endian to system-endian (32-bit)
inline uint64 b_stol64(uint64 system);		//! System-endian to little-endian (64-bit)
inline uint64 b_ltos64(uint64 little);		//! Little-endian to system-endian (64-bit)

/* big-endian */
inline uint16 b_stob16(uint16 system);		//! System-endian to big-endian (16-bit)
inline uint16 b_btos16(uint16 big);			//! Big-endian to system-endian (16-bit)
inline uint32 b_stob32(uint32 system);		//! System-endian to big-endian (32-bit)
inline uint32 b_btos32(uint32 big);			//! Big-endian to system-endian (32-bit)
inline uint64 b_stob64(uint64 system);		//! System-endian to big-endian (64-bit)
inline uint64 b_btos64(uint64 big);			//! Big-endian to system-endian (64-bit)

#endif
#endif
