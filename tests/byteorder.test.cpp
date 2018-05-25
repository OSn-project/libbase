#include <stdio.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>

#include "../include/osndef.h"
#include "../include/byteorder.h"

SUITE(Byteorder)
{	
	static uint8 number16[]         = {0x01, 0x02};
	static uint8 number16_swapped[] = {0x02, 0x01};
	static uint8 number32[]         = {0x01, 0x02, 0x03, 0x04};
	static uint8 number32_swapped[] = {0x04, 0x03, 0x02, 0x01};
	static uint8 number64[]         = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	static uint8 number64_swapped[] = {0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};
	
	/* Byte swapping */
	
	TEST (b_byteswap16)
	{
		uint16 data = b_byteswap16(*((uint16 *) number16));
		CHECK(! memcmp(&data, number16_swapped, sizeof(uint16)));
	}
	
	/* Big endian */
	
	TEST (b_btos16)
	{
		CHECK(b_btos16(*((uint16 *) number16)) == 258);
	}
	
	TEST (b_stob16)
	{
		uint16 num = b_stob16(258);
		
		CHECK(! memcmp(&num, number16, sizeof(uint16)));
	}
	
	TEST (b_btos32)
	{
		CHECK(b_btos32(*((uint32 *) number32)) == 16909060);
	}

	TEST (b_btos64)
	{
		CHECK(b_btos64(*((uint64 *) number64)) == 72623859790382856);
	}
}
