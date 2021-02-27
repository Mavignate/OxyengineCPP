#include "Audio.hpp"
#include "WAV.hpp"
#include <cstring>
#include <cassert>

namespace
{

std::unique_ptr<Audio> audio_ptr;

}

constexpr unsigned Audio::PLAY_SAMPLES;

constexpr unsigned Audio::LOAD_SAMPLES;

Voice::Voice()
:	bufferPos(0),
	bufferLen(0),
	ready(false)
{
	output.resize(Audio::PLAY_SAMPLES * audio().getChannelsNumber(), 0.f);
}

Voice::Voice(const std::string& file)
:	Voice()
{
	loadFromFile(file);
}

Voice::~Voice()
{}

bool Voice::loadFromFile(const std::string& file)
{
	bufferPos = 0;
	bufferLen = 0;
	ready = false;
	source.reset();
	buffer.clear();

	if(file.substr(file.size() - 4, 4) == ".wav" || file.substr(file.size() - 5, 5) == ".wave")
		source = std::make_unique<WAV>(file);
	else
	{
		fprintf(stderr, "Unsupported audio format\n");
		return false;
	}

	if(!source->isValid()) source.reset();
	return ready = (bool)source;
}

void Voice::startPlaying()
{
	if(source)
	{
		source->startReading();
		refillBuffer();
	}
}

void Voice::stopPlaying()
{
	bufferPos = 0;
	bufferLen = 0;
	source->stopReading();
	std::memset(output.data(), 0, sizeof(float) * Audio::PLAY_SAMPLES);
}

bool Voice::isPlaying() const
{
	if(!isReady())                           return false;
	if(source->finished() && bufferLen == 0) return false;
	return true;
}

const float* Voice::getData()
{
	if(!isReady() || !isPlaying()) return output.data();

	unsigned filled = 0;
	unsigned size = sizeof(float) * Audio::PLAY_SAMPLES;
	uint8_t* out = reinterpret_cast<uint8_t*>(output.data());

	if(bufferPos == bufferLen && source->finished())
	{
		bufferPos = 0;
		bufferLen = 0;
		std::memset(out, 0, size);
		return output.data();
	}

	while(filled < size)
	{
		if(bufferPos < bufferLen)
		{
			unsigned copy = std::min(bufferLen - bufferPos, size);
			std::memcpy(out + filled, buffer.data() + bufferPos,
					copy);
			bufferPos  += copy;
			filled     += copy;
		}
		else if(bufferPos == bufferLen)
		{
			if(source->finished()) break;
			refillBuffer();
		}
	}

	if(filled < size) std::memset(out + filled, 0, size - filled);

	return output.data();
}

bool Voice::isReady() const
{
	return ready;
}

void Voice::refillBuffer()
{
	if(!isReady())          return;
	if(source->finished())  return;

	SDL_AudioCVT cvt;
	SDL_BuildAudioCVT(&cvt,
			source->getFormat(),  source->getChannels(),        source->getSampleRate(),
			AUDIO_F32,            audio().getChannelsNumber(),  audio().getSampleRate());

	cvt.len = source->getBytesPerSample() * Audio::LOAD_SAMPLES;
	if((int)buffer.size() < cvt.len * cvt.len_mult) buffer.resize(cvt.len * cvt.len_mult);
	source->getData(buffer.data());
	cvt.buf = buffer.data();

	if(SDL_ConvertAudio(&cvt) < 0)
	{
		fprintf(stderr, "Failed to convert audio format\nSDL notice: %s", SDL_GetError());
		return;
	}

	bufferPos = 0;
	bufferLen = cvt.len_cvt;
}

void Audio::audioCallback(void* userdata, uint8_t* stream, int len)
{
	if(len / (audio().deviceSpec.channels * sizeof(float)) != PLAY_SAMPLES) throw audio_error("Unexpected error: incorrect number of samples");
	if(audio().voice)
	{
		if(audio().voice->isPlaying())
			std::memcpy(stream, audio().voice->getData(), audio().getChannelsNumber() * sizeof(float) * PLAY_SAMPLES);
		else SDL_PauseAudioDevice(audio().deviceID, 1);
	}
}

Audio::Audio(ChannelFormat channels, unsigned freq)
:	deviceID(0),
	voice(nullptr)
{
	if(audio_ptr)
		throw audio_error("Additional Audio object created");

	if(!SDL_WasInit(SDL_INIT_AUDIO))
		if(SDL_InitSubSystem(SDL_INIT_AUDIO))
			throw audio_error(std::string("Failed to initialize SDL_AUDIO\nSDL notice: ") + SDL_GetError());

	deviceSpec.freq = freq;

	setChannelFormat(channels);
}

Audio::~Audio()
{
	if(deviceID) SDL_CloseAudioDevice(deviceID);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void Audio::play(Voice& voice)
{
	if(voice.isReady())
	{														(std::string("lol") + "lol").length();
		SDL_LockAudioDevice(deviceID);
		if(this->voice) this->voice->stopPlaying();
		this->voice = &voice;
		this->voice->startPlaying();
		SDL_UnlockAudioDevice(deviceID);
		SDL_PauseAudioDevice(deviceID, 0);
	}
}

bool Audio::isPlaying() const
{
	if(!voice) return false;
	return voice->isPlaying();
}

void Audio::setChannelFormat(ChannelFormat format)
{
	if(voice) voice->stopPlaying();
	if(deviceID) SDL_CloseAudioDevice(deviceID);

	SDL_AudioSpec spec;
	spec.freq = deviceSpec.freq;
	spec.format = AUDIO_F32;
	spec.channels = static_cast<unsigned>(format);
	spec.samples = PLAY_SAMPLES;
	spec.callback = Audio::audioCallback;

	deviceID = SDL_OpenAudioDevice(NULL, 0, &spec, &deviceSpec, 0);

	if(!deviceID)
		throw audio_error(std::string("Failed to open an audio device\nSDL notice: ") + SDL_GetError());
}

unsigned Audio::getChannelsNumber() const
{
	return deviceSpec.channels;
}

unsigned Audio::getSampleRate() const
{
	return deviceSpec.freq;
}

void initAudio(Audio::ChannelFormat channels, unsigned samplerate)
{
	if(audio_ptr) return;
	audio_ptr = std::make_unique<Audio>(channels, samplerate);
}

Audio& audio()
{
	if(!audio_ptr) initAudio();
	return *audio_ptr;
}
