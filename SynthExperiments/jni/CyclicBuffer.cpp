/*
 * CyclicBuffer.cpp
 *
 *  Created on: 2 Jul 2014
 *      Author: Kris Popat
 *
 *      Implementation of a circular binary stream for
 *      use in asynchronous situations.
 */

#include <CyclicBuffer.h>
#include <time.h>
#include <cstdint>
#include <cstring>

CyclicBuffer::CyclicBuffer()
{
	readPosition = 0;
	writePosition = 0;
}


int CyclicBuffer::NumberOfBytesToRead( )
{
	// because these are unsigned m if the result is negative
	// m will be a wrap around of the negative result
	// mask this against the buffer size to find the actual bytes
	// to be read.
	unsigned int m = writePosition - readPosition;
	return m & ( kBufferSize - 1 );
}

int CyclicBuffer::SizeOfAvailableSpace()
{
	// as above but this calculates the remaining space - 1
	unsigned int m = readPosition - writePosition - 1;
	return m & ( kBufferSize - 1 );
}

int CyclicBuffer::Read( int size, uint8_t* bytes )
{
	unsigned int readIndex = readPosition;
	__sync_synchronize();
	int rem = kBufferSize - readIndex;

	int sectionSize = ( size < rem ) ? size: rem;
	memcpy ( bytes, buffer + readIndex, sectionSize );
	if ( size > sectionSize ) {
		memcpy ( bytes + sectionSize, buffer, size - sectionSize );
	}
	__sync_synchronize();
	readPosition = (readIndex + size) & (kBufferSize - 1);
	return size;

}

int CyclicBuffer::Write( int size, const uint8_t* bytes )
{
	int r = size;
	while ( r > 0 ) {
		unsigned int readIndex = readPosition;
		unsigned int writeIndex = writePosition;
		int v = ( readIndex - writeIndex - 1) & (kBufferSize - 1 );
		if ( v == 0 ) {
			// sleep for a bit
			struct timespec s;
			s.tv_sec = 0;
			s.tv_nsec = 1000000; // one millisecond
			nanosleep( &s, NULL );
		}
		else {
			int rs = ( r < v )? r:v;
			int rem = kBufferSize - writeIndex;
			int sectionSize = ( rs < rem ) ? rs : rem;
			memcpy ( buffer + writePosition, bytes, sectionSize );
			if ( rs > sectionSize ) {
				memcpy ( buffer, bytes + sectionSize, rs - sectionSize );
			}
			__sync_synchronize();
			writePosition = (writeIndex + rs) & ( kBufferSize - 1 );
			r -= rs;
			bytes += rs;
		}
	}
	return size;
}
