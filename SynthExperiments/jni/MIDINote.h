/*
 * MIDINote.h
 *
 *  Created on: 18 Jun 2014
 *      Author: Kris Popat
 */

#ifndef MIDINOTE_H_
#define MIDINOTE_H_

#include <stdint.h>

class Envelope;

class MIDINote {
public:
				MIDINote( Envelope* inEnvelope, double inEnvelopePhaseDelta );
				MIDINote( uint8_t* msg, int msgLength, Envelope* inEnvelope, double inEnvelopePhaseDelta );
	virtual 	~MIDINote();

	bool		updateFromMIDIMessage(uint8_t* msg, int msgLength);
	double		computeFrequency();
	void		setNoteFromFrequency ( double frequency );

	//void		computeSamples ( uint32_t bufferSize, int16_t *buffer );

	double		calculateEnvelopeFactorPerSample ( );

	void		resetNoteData( );

	bool		isKeydown( ) const;
	void		setKeydown( bool keydown );
	bool 		isActive( ) const;
	void 		setActive( bool active );
	uint8_t 	getChannel( ) const;
	float 		getDetune( ) const;
	void 		setDetune( float detune );
	uint8_t 	getPitch( ) const;
	uint8_t 	getVelocity( ) const;



	// public properties - accessed in real time.

	double		frequency;
	double		oscilatorPhase;

	uint8_t		envelopePhase;
	double		envelopePhasePosition;
	double		envelopePhaseDelta;
	double		envelopeFactor;


private:

	uint8_t		pitch; // midi pitch as in 0x3C is middle c
	uint8_t		channel;
	uint8_t		velocity;
	float		detune;
	bool		keydown;
	bool		active;
	Envelope*	envelope;



};

#endif /* MIDINOTE_H_ */
