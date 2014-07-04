/*
 * CyclicBuffer.h
 *
 *  Created on: 2 Jul 2014
 *      Author: Kris Popat
 */

#ifndef CYCLICBUFFER_H_
#define CYCLICBUFFER_H_

#include <cstdint>

class CyclicBuffer {
public:
				CyclicBuffer( );
	int			NumberOfBytesToRead( );
	int			SizeOfAvailableSpace( );
	int			Read( int size, uint8_t* bytes );
	int			Write( int size, const uint8_t* bytes );

private:
	static const int		kBufferSize = 0x2000;
	uint8_t					buffer[kBufferSize];
	volatile unsigned int	writePosition;
	volatile unsigned int	readPosition;
};

#endif /* CYCLICBUFFER_H_ */
