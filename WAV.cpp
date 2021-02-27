#include "WAV.hpp"
#include <cstring>

namespace
{

inline uint32_t convertToNumeric(const char name[4])
{
	return (name[0] << 0) | (name[1] << 8) | (name[2] << 16) | (name[3] << 24);
}

}

bool WAV::verify() const
{
	return
			riff == convertToNumeric("RIFF") &&
			size > 0 &&
			wave == convertToNumeric("WAVE") &&
			dataPos > 0 &&
			sampleCount > 0 &&
			compression > 0 &&
			channels > 0 &&
			sampleRate > 0 &&
			bytesPerSecond > 0 &&
			bytesPerSample > 0 &&
			bitsPerSample > 0 && bitsPerSample % 8 == 0;
}

WAV::WAV(const std::string& filename)
:	file(filename),
	riff(0),
	size(0),
	wave(0),
	valid(false),
	dataPos(0),
	sampleCount(0),
	currentSample(0),
	compression(0),
	channels(0),
	sampleRate(0),
	bytesPerSecond(0),
	bytesPerSample(0),
	bitsPerSample(0)
{
	stream.open(file, std::ios_base::in | std::ios_base::binary);
	if(!stream.good())
	{
		fprintf(stderr, "Failed to open '%s'", file.c_str());
		return;
	}

	auto load = [&](void* ptr, unsigned size) -> bool{
		stream.read((char*)ptr, size);
		if(!stream.good())
		{
			fprintf(stderr, "'%s' is not a Waveform file\n", file.c_str());
			stream.close();
			return false;
		}
		return true;
	};

	if(!load(&riff, 12)) return;

	uint32_t buffer[2];
	bool data = false, fmt_ = false;

	while(!(data && fmt_))
	{
		if(!load(buffer, 8)) return;
		if(buffer[0] == convertToNumeric("data"))
		{
			if(data)
			{
				stream.close();
				fprintf(stderr, "'%s' is not a Waveform file\n", file.c_str());
				return;
			}
			data = true;
			dataPos = stream.tellg();
			sampleCount = buffer[1];
			stream.seekg(buffer[1], std::ios_base::cur);
		}
		else if(buffer[0] == convertToNumeric("fmt "))
		{
			if(fmt_)
			{
				stream.close();
				fprintf(stderr, "'%s' is not a Waveform file\n", file.c_str());
				return;
			}
			fmt_ = true;
			unsigned pos = stream.tellg();

			load(&compression, 16);
			if(compression != 1 || bitsPerSample == 24)
			{
				fprintf(stderr, "Unsupported Waveform format\n");
				return;
			}

			stream.seekg(pos + buffer[1], std::ios_base::beg);
		}
	}
	stream.close();

	if(sampleCount % bytesPerSample == 0) sampleCount /= bytesPerSample;
	else
	{
		fprintf(stderr, "'%s' is not a Waveform file\n", file.c_str());
		return;
	}

	if(!verify())
	{
		fprintf(stderr, "'%s' is not a Waveform file\n", file.c_str());
		return;
	}

	this->buffer.resize(bytesPerSample * Audio::LOAD_SAMPLES, 0.f);
	valid = true;
}

WAV::~WAV()
{
	if(stream.is_open()) stream.close();
}

unsigned WAV::getChannels() const
{
	return channels;
}

SDL_AudioFormat WAV::getFormat() const
{
	switch(bitsPerSample)
	{
	case 8:   return AUDIO_U8;
	case 16:  return AUDIO_S16;
	case 32:  return AUDIO_S32;
	}

	throw audio_error("Requested format from an invalid audio source");
}

unsigned WAV::getBytesPerSample() const
{
	return bytesPerSample;
}

unsigned WAV::getSampleRate() const
{
	return sampleRate;
}

void WAV::getData(uint8_t* dest)
{
	if(sampleCount - currentSample < 4096 || finished()) std::memset(dest, 0, bytesPerSample * 4096);
	if(finished()) return;

	stream.read((char*)dest, std::min((sampleCount - currentSample), Audio::LOAD_SAMPLES) * bytesPerSample);

	if(sampleCount - currentSample <= Audio::LOAD_SAMPLES)
	{
		currentSample = 0;
		stream.close();
	}
	else currentSample += Audio::LOAD_SAMPLES;
}

void WAV::startReading()
{
	if(stream.is_open()) stream.close();
	stream.open(file, std::ios_base::in | std::ios_base::binary);
	stream.seekg(dataPos, std::ios_base::beg);
	currentSample = 0;

	if(!stream.good())
	{
		if(stream.is_open()) stream.close();
		fprintf(stderr, "Failed to read file '%s'\n", file.c_str());
	}
}

void WAV::stopReading()
{
	stream.close();
}

bool WAV::isValid() const
{
	return valid;
}

bool WAV::finished()
{
	return !stream.is_open();
}
