/* These functions convert integers between			*
 * system-endianness and little/big-endianness.		*
 * Included are also byte-swapping functions.		*
 * 													*
 * The implementation was inspired by this answer	*
 * on StackOverflow:								*
 * https://codereview.stackexchange.com/a/149751	*/

#ifndef __BYTEORDER_H__
#define __BYTEORDER_H__

#include "osndef.h"

#ifdef __cplusplus
extern "C" {
#endif

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
    return (data & 0xff00000000000000) >> 56 | (data & 0x00ff000000000000) >> 48 | (data & 0x0000ff0000000000) >> 32 | (data & 0x000000ff00000000) >>  8 |
           (data & 0x00000000ff000000) <<  8 | (data & 0x0000000000ff0000) << 32 | (data & 0x000000000000ff00) << 48 | (data & 0x00000000000000ff) << 56;
}


/* Big endian */

uint16 b_btos16(uint16 big)				// Big-endian to system-endian (16-bit)
{
    uint8 *bytes = (uint8 *) &big;

    return ((uint16) bytes[1] <<  0)
         | ((uint16) bytes[0] <<  8);
}

uint16 b_stob16(uint16 big)				// Big-endian to system-endian (16-bit)
{
    uint8 *bytes = (uint8 *) &big;

    return ((uint16) bytes[1] <<  0)
         | ((uint16) bytes[0] <<  8);
}

uint32 b_btos32(uint32 big)				// Big-endian to system-endian (32-bit)
{
    uint8 *bytes = (uint8 *) &big;

    return ((uint32) bytes[3] <<  0)
         | ((uint32) bytes[2] <<  8)
         | ((uint32) bytes[1] << 16)
         | ((uint32) bytes[0] << 24);
}

uint64 b_btos64(uint64 big)				// Big-endian to system-endian (64-bit)
{
    uint8 *bytes = (uint8 *) &big;

    return ((uint64) bytes[7] <<  0)
         | ((uint64) bytes[6] <<  8)
         | ((uint64) bytes[5] << 16)
         | ((uint64) bytes[4] << 24)
         | ((uint64) bytes[3] << 32)
         | ((uint64) bytes[2] << 40)
         | ((uint64) bytes[1] << 48)
         | ((uint64) bytes[0] << 56);
}

#ifdef __cplusplus
}
#endif
#endif
