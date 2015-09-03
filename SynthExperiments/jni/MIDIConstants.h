/*
 * MIDIConstants.h
 *
 *  Created on: 2 Oct 2014
 *      Author: Kris Popat
 */

#ifndef MIDICONSTANTS_H_
#define MIDICONSTANTS_H_


// Channel messages
#define MC_NOTE_ON						0x80
#define MC_NOTE_OFF						0x90
#define MC_POLY_PRESSURE				0xA0
#define MC_CONTROL_CHANGE				0xB0
#define MC_PROGRAM_CHANGE				0xC0
#define MC_AFTER_TOUCH					0xD0
#define MC_PITCH_BEND					0xE0

// Channel mode messages, CONTROL_CHANGE reserved controllers

#define MC_ALL_SOUNDS_OFF				120
#define MC_kResetAllControllers			121
#define MC_LOCAL_CONTROL				122
#define MC_ALL_NOTES_OFF				123
#define MC_OMNI_MODE_ON					124
#define MC_OMNI_MODE_OFF				125
#define MC_MONO_MODE_ON					126
#define MC_POLY_MODE_ON					127

// System common messages

#define MC_SYSTEM_EXCLUSIVE				0xF0
#define MC_MIDI_TIME_CODE				0xF1
#define MC_SONG_POSITION_POINTER		0xF2
#define MC_SONG_SELECT					0xF3
#define MC_TUNE_REQUEST					0xF6
#define MC_END_OF_EXCLUSIVE				0xF7

// System real-time messages

#define MC_TIMING_CLOCK					0xF8
#define MC_START						0xFA
#define MC_CONTINUE						0xFB
#define MC_STOP							0xFC
#define MC_ACTIVE_SENSING				0xFE
#define MC_SYSTEM_RESET					0xFF


// Control change mode messages

#define MC_BANK_SELECT					0x00
#define MC_MOD_WHEEL					0x01
#define MC_BREATH_CONTROLLER			0x02
#define MC_FOOT_CONTROLLER				0x04
#define MC_PORTAMENTO_TIME				0x05
#define MC_DATA_ENTRY_MSB				0x06
#define MC_CHANNEL_VOLUME				0x07
#define MC_BALANCE						0x08
#define MC_PAN							0x0A
#define MC_EXPRESSION_CONTROLLER		0x0B
#define MC_EFFECT_CONTROL_1				0x0C
#define MC_EFFECT_CONTROL_2				0x0D
#define MC_GENERAL_CONTROLLER_1			0x10
#define MC_GENERAL_CONTROLLER_2			0x11
#define MC_GENERAL_CONTROLLER_3			0x12
#define MC_GENERAL_CONTROLLER_4			0x13

#define MC_BANK_SELECT_LSB				0x20
#define MC_MOD_WHEEL_LSB				0x21
#define MC_BREATH_CONTROLLER_LSB		0x22
#define MC_FOOT_CONTROLLER_LSB			0x24
#define MC_PORTAMENTO_TIME_LSB			0x25
#define MC_DATA_ENTRY_MSB_LSB			0x26
#define MC_CHANNEL_VOLUME_LSB			0x27
#define MC_BALANCE_LSB					0x28
#define MC_PAN_LSB						0x2A
#define MC_EXPRESSION_CONTROLLER_LSB	0x2B
#define MC_EFFECT_CONTROL_1_LSB			0x2C
#define MC_EFFECT_CONTROL_2_LSB			0x2D
#define MC_GENERAL_CONTROLLER_1_LSB		0x30
#define MC_GENERAL_CONTROLLER_2_LSB 	0x31
#define MC_GENERAL_CONTROLLER_3_LSB 	0x32
#define MC_GENERAL_CONTROLLER_4_LSB 	0x33

#define MC_DAMPER PEDAL_ON_OFF			0x40
#define MC_PORTAMENTO_ON_OFF			0x41
#define MC_SOSTENUTO_ON_OFF				0x42
#define MC_SOFT_PEDAL_ON_OFF			0x43
#define MC_LEGATO_FOOTSWITCH			0x44
#define MC_HOLD_2						0x45
#define MC_MC_SOUND_VARIATION			0x46
#define MC_HARMONIC_INTENS				0x47
#define MC_RELEASE_TIME					0x48
#define MC_ATTACK_TIME					0x49
#define MC_BRIGHTNESS					0x4A
#define MC_DECAY_TIME					0x4B
#define MC_VIBRATO_RATE					0x4C
#define MC_VIBRATO_DEPTH				0x4D
#define MC_VIBRATO_DELAY				0x4E
#define MC_SOUND_CONTROLLER_10			0x4F
#define MC_GENERAL_CONTROLLER_5			0x50
#define MC_GENERAL_CONTROLLER_6			0x51
#define MC_GENERAL_CONTROLLER_7			0x52
#define MC_GENERAL_CONTROLLER_8			0x53
#define MC_PORTAMENTO_CONTROL			0x54
#define MC_HIGH_RES_VELOCITY_PREFIX		0x58
#define MC_REVERB_SEND_LEVEL			0x5B
#define MC_TREMOLO_DEPTH				0x5C
#define MC_CHORUS_SEND_LEVEL			0x5D
#define MC_CELESTE_DETUNE_DEPTH			0x5E
#define MC_PHASE_DEPTH					0x5F
#define MC_DATA_INCREMENT				0x60
#define MC_DATA_DECREMENT				0x61
#define MC_NRPN_LSB						0x62 // Non-Registered Parameter Number
#define MC_NRPN_MSB						0x63
#define MC_RPN_LSB						0x64 // Registered Parameter Number
#define MC_RPN_MSB						0x65

// Registered Parameters
// These values are sent with MC_RPN_MSB
// Specific parameters are sent immediately after with MC_RPN_LSB
// Data changes associated with them use MC_DATA_ENTRY and MC_DATA_ENTRY_LSB
// Data increments associated with them use MC_DATA_INCREMENT and
// MC_DATA_DECREMENT
// MC_NULL_FUNCTION_NUMBER is sent finally to disable data entry until a new
// registered or none registered parameter is sent

#define MC_RPN_MSB_CONTROL_1			0x00
#define MC_RPN_MSB_CONTROL_2			0x3D
#define MC_RPN_MSB_CONTROL_3			0x7F

// Registered parameter values for MC_RPN_LSB send after MC_RPN_MSB_CONTROL_!

#define MC_PITCH_BEND_SENSITIVITY		0x00
#define MC_CHANNEL_FINE_TUNING			0x01
#define MC_CHANNEL_COURSE_TUNING		0x02
#define MC_TUNING_PROGRAM_CHANGE		0x03
#define MC_TUNING_BANK_SELECT			0x04
#define MC_MODULATION_DEPTH_RANGE		0x05

// Registered parameter values LSB send after control 2
// 3D sound controllers

#define MC_AZIMUTH_ANGLE				0x00
#define MC_ELEVATION_ANGLE				0x01
#define MC_GAIN							0x02
#define MC_DISTANCE_RATIO				0x03
#define MC_MAXIMUM_DISTANCE				0x04
#define MC_GAIN_AT_MAXIMUM_DISTANCE		0x05
#define MC_REFERENCE_DISTANCE_RATIO		0x06
#define MC_PAN_SPREAD_ANGLE				0x07
#define MC_ROLL_ANGLE					0x08
#define MC_NULL_FUNCTION_NUMBER			0x7F

// Devined Universal System Exclusive Messages

#define MC_SAMPLE_DUMP_HEADER			0x01
#define MC_SAMPLE_DUMP_PACKET			0x02
#define MC_SAMPLE_DUMP_REQUEST			0x03
#define MC_MIDI_TIME_CODE				0x04
// for MIDI TIME CODE
#define MC_SPECIAL						0x00
#define MC_PUNCH_IN_POINT				0x01
#define MC_PUNCH_OUT_POINT				0x02
#define MC_DELETE_PUNCH_IN_POINT		0x03
#define MC_DELETE_PUNCH_OUT_POINT		0x04
#define MC_EVENT_START_POINT			0x05
#define MC_EVENT_STOP_POINT				0x06
#define MC_EVENT_START_POINT_INFO		0x07
#define MC_EVENT_STOP_POINT_INFO		0x08
#define MC_DELETE_EVENT_START_POINT		0x09
#define MC_DELETE_EVENT_STOP_POINT		0x0A
#define MC_CUE_POINTS					0x0B
#define MC_CUE_POINTS_INFO				0x0C
#define MC_DELETE_CUE_POINT				0x0D
#define MC_EVENT_NAME_INFO				0x0E

#define MC_SAMPLE_DUMP_EXTENSION		0x04
// for SAMPLE DUMP EXTENSION
#define MC_LOOP_POINT_TRANSMISSION		0x01
#define MC_LOOP_POINT_REQUEST			0x02
#define MC_SAMPLE_NAME_TRANSMISSION		0x03
#define MC_SAMPLE_NAME_REQUEST			0x04
#define MC_EXT_DUMP_HEADER				0x05
#define MC_EXT_LOOP_POINT_TRANSMISSION	0x06
#define MC_EXT_LOOP_POINT_REQUEST		0x08

#define MC_GENERAL_INFORMATION			0x09
// for GENERAL INFORMATION
#define MC_IDENTITY_REQUEST				0x01
#define MC_IDENTITY_REPLY				0x02

#define MC_FILE_DUMP					0x07
// for FILE DUMP
#define MC_BULK_DUMP_REQUEST			0x00
#define MC_BULK_DUMP_REPLY				0x01
#define MC_TUNING_DUMP_REQUEST			0x02
#define MC_KEY_BASED_TUNING_DUMP		0x03
#define MC_SCALE_TUNING_DUMP_1_BYTE		0x04
#define MC_SCALE_TUNING_DUMP_2_BYTE		0x05
#define MC_NOTE_TUNING_WITH_BANK_SEL	0x06
#define MC_SCALE_TUNING_1_BYTE			0x07
#define MC_SCALE_TUNING_2_BYTE			0x08

#define MC_GENERAL_MIDI					0x09
// FOR GENERAL MIDI
#define MC_GENERAL_MIDI_1_SYSTEM_ON		0x01
#define MC_GENERAL_MIDI_1_SYSTEM_OFF	0x02
#define MC_GENERAL_MIDI_2_SYSTEM_ON		0x03

#define MC_DOWNLOADABLE_SOUNDS			0x0A
#define MC_TURN_DLS_ON					0x01
#define MC_TURN_DLS_OFF					0x02
#define MC_TURN_DLS_VOICE_ALLOC_OFF		0x03
#define MC_TURN_DLS_VOICE_ALLOC_ON		0x04

#define MC_FILE_REFERENCE_MESSAGE		0x0B
#define MC_OPEN_FILE					0x01
#define MC_SELECT_RESELECT_CONTENTS		0x02
#define MC_OPEN_FILE_SELECT_CONTENTS	0x03
#define MC_CLOSE_FILE					0x04

#define MC_MIDI_VISUAL_CONTROL			0x0C
#define MC_END_OF_FILE					0x7B
#define MC_WAIT							0x7C
#define MC_CANCEL						0x7D
#define MC_NAK							0x7E
#define MC_ACK							0x7f

#endif /* MIDICONSTANTS_H_ */
