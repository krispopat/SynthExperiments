LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    :=	KPSynth
LOCAL_SRC_FILES :=	AndroidGlue.cpp \
					OpenSLManager.cpp \
					Synthesizer.cpp \
					WaveTable.cpp \
					Envelope.cpp \
					Ramp.cpp \
					ADSREnvelope.cpp \
					MIDINote.cpp \
					CyclicBuffer.cpp

LOCAL_LDLIBS    += -lOpenSLES
# for logging
LOCAL_LDLIBS    += -llog
# for native asset manager
LOCAL_LDLIBS    += -landroid
include $(BUILD_SHARED_LIBRARY)