#pragma once

#include "Audio.hpp"
#include <fstream>

class WAV
:	public VoiceSource
{
	std::ifstream stream;

	const std::string file;

	std::vector<uint8_t> buffer;

	uint32_t riff;
	uint32_t size;
	uint32_t wave;

	bool valid;

	unsigned dataPos;
	unsigned sampleCount;
	unsigned currentSample;

	uint16_t compression;
	uint16_t channels;
	uint32_t sampleRate;
	uint32_t bytesPerSecond;
	uint16_t bytesPerSample;
	uint16_t bitsPerSample;

	bool verify() const;

public:

	WAV(const std::string& filename);

	virtual ~WAV();

	virtual unsigned getChannels() const;

	virtual SDL_AudioFormat getFormat() const;

	virtual unsigned getBytesPerSample() const;

	virtual unsigned getSampleRate() const;

	virtual void getData(uint8_t* buffer);

	virtual void startReading();

	virtual void stopReading();

	virtual bool isValid() const;

	virtual bool finished();
};
