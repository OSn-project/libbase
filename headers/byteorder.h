// !!! THE IMPLEMENTATION OF THIS NEEDS TO BE RE-THOUGHT !!!

/*!These functions convert integers between		*
 * system-endianness and little/big-endianness.	*/

#ifndef __BSTRING_H__
#define __BSTRING_H__

#include "bdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*              Endianness Probing				*
 * We determine the endianness of the system by	*
 * setting each byte of a 32-bit integer indep-	*
 * endently (using a union), and then comparing	*
 * how the bytes are interpreted.				*/

const static union
{
	uint8  bytes[4];
	uint32 value;
} b_endianness_probe = {
	{0x00, 0x01, 0x02, 0x03}
};

#define CPU_BIG_ENDIAN    (b_endianness_probe.value == 0x00010203ul)
#define CPU_LITTLE_ENDIAN (b_endianness_probe.value == 0x03020100ul)		// On Intel this should be 1 because intel is little-endian.

/* Little endian */
inline uint16 b_stol16(uint16 system);		//! System-endian to little-endian (16-bit)
inline uint16 b_ltos16(uint16 little);		//! Little-endian to system-endian (16-bit)
inline uint32 b_stol32(uint32 system);		//! System-endian to little-endian (32-bit)
inline uint32 b_ltos32(uint32 little);		//! Little-endian to system-endian (32-bit)
inline uint64 b_stol64(uint64 system);		//! System-endian to little-endian (64-bit)
inline uint64 b_ltos64(uint64 little);		//! Little-endian to system-endian (64-bit)

/* Big endian */
inline uint16 b_stob16(uint16 system);		//! System-endian to big-endian (16-bit)
inline uint16 b_btos16(uint16 big);			//! Big-endian to system-endian (16-bit)
inline uint32 b_stob32(uint32 system);		//! System-endian to big-endian (32-bit)
inline uint32 b_btos32(uint32 big);			//! Big-endian to system-endian (32-bit)
inline uint64 b_stob64(uint64 system);		//! System-endian to big-endian (64-bit)
inline uint64 b_btos64(uint64 big);			//! Big-endian to system-endian (64-bit)

/* Byte-swapping */
inline uint16 b_byteswap16(uint16 data);		//! Byte swap (16-bit)
inline uint32 b_byteswap32(uint32 data);		//! Byte swap (32-bit)
inline uint64 b_byteswap64(uint64 data);		//! Byte swap (64-bit)

/** Implementation **/

/* Big endian */
uint16 b_stob16(uint16 system)
{
	/* System-endian to big-endian (16-bit) */

	if (CPU_BIG_ENDIAN)
	{
		return system;			// <- No need to change anything if the system is already big endian.
	}
	else if (CPU_LITTLE_ENDIAN)
	{
		return (system & 0xff00) >> 8 | (system & 0x00ff) << 8;
	}
	
	return system;
}

uint16 b_btos16(uint16 big)
{
	/* Big-endian to system-endian (16-bit) */
	
	if (CPU_BIG_ENDIAN)
	{
		return big;
	}
	else if (CPU_LITTLE_ENDIAN)
	{
		return (big & 0xff00) >> 8 | (big & 0x00ff) << 8;
	}
	
	return big;
}

/* Byte-swapping */
uint16 b_byteswap16(uint16 data)
{
	return (data & 0xff00) >> 8 | (data & 0x00ff) << 8;
}

uint32 b_byteswap32(uint32 data)
{
	return (data & 0xff000000) >> 24 | (data & 0x00ff0000) >> 8 | (data & 0x0000ff00) << 8 | (data & 0x000000ff) << 24;
}

uint64 b_byteswap64(uint64 data)
{
	return	(data & 0xff00000000000000) >> 56 | (data & 0x00ff000000000000) >> 48 | (data & 0x0000ff0000000000) >> 32 | (data & 0x000000ff00000000) >>  8 |
			(data & 0x00000000ff000000) <<  8 | (data & 0x0000000000ff0000) << 32 | (data & 0x000000000000ff00) << 48 | (data & 0x00000000000000ff) << 56;
}

#ifdef __cplusplus
}
#endif
#endif
