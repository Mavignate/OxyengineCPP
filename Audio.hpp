#pragma once

#include <vector>
#include <stdexcept>
#include <memory>
#include <SDL2/SDL.h>
//TODO update voice buffer size when channel count is changed

struct audio_error
:	public std::runtime_error
{
	inline audio_error(const std::string& what)
	:	std::runtime_error(what)
	{}

	inline audio_error(const char* what)
	:	std::runtime_error(what)
	{}

	virtual ~audio_error(){}
};

struct VoiceSource
{
	virtual ~VoiceSource(){}

	virtual unsigned getChannels() const = 0;

	virtual SDL_AudioFormat getFormat() const = 0;

	virtual unsigned getBytesPerSample() const = 0;

	virtual unsigned getSampleRate() const = 0;

	virtual void getData(uint8_t* dest) = 0;

	virtual void startReading() = 0;

	virtual void stopReading() = 0;

	virtual bool isValid() const = 0;

	virtual bool finished() = 0;
};

struct Voice
{
	friend class Audio;

	Voice();

	Voice(const std::string& file);

	Voice(const Voice&) = delete;
	Voice& operator=(const Voice&) = delete;

	~Voice();

	bool loadFromFile(const std::string& file);

	void startPlaying();

	void stopPlaying();

	bool isPlaying() const;

private:

	std::vector<uint8_t> buffer;

	unsigned bufferPos;
	unsigned bufferLen;

	std::vector<float> output;

	std::unique_ptr<VoiceSource> source;

	bool ready;

	const float* getData();

	bool isReady() const;

	void refillBuffer();
};

class Audio
{
	SDL_AudioDeviceID deviceID;

	SDL_AudioSpec deviceSpec;

	Voice* voice;

	static void audioCallback(void* userdata, uint8_t* stream, int len);

public:

	static constexpr unsigned PLAY_SAMPLES = 1024u;

	static constexpr unsigned LOAD_SAMPLES = PLAY_SAMPLES * 4u;

	enum ChannelFormat
	{
		Mono          = 1,
		Stereo        = 2,
		Quad          = 4,
		Surround_5_1  = 6,
		Surround_7_1  = 8,

		Default = Stereo
	};

	Audio(ChannelFormat channels, unsigned freq);

	Audio(const Audio&) = delete;
	Audio& operator=(const Audio&) = delete;

	~Audio();

	void play(Voice& voice);

	bool isPlaying() const;

	void setChannelFormat(ChannelFormat format);

	unsigned getChannelsNumber() const;

	unsigned getSampleRate() const;
};

void initAudio(Audio::ChannelFormat channels = Audio::Default, unsigned samplerate = 22050);

Audio& audio();
