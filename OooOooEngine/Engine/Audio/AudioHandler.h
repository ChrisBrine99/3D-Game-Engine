#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <glm/glm.hpp>
#include <fmod.hpp>
#include <string>
#include <memory>
#include <map>
#include "../Core/DebugLogger.h"

class AudioHandler {
private:
	friend class AudioSourceComponent;

	static std::map<std::string, FMOD::Sound*> sounds;
	static std::map<int, FMOD::Channel*> channels;

	static std::unique_ptr<AudioHandler> instance;
	friend std::default_delete<AudioHandler>;

	FMOD::System* system;
	int channelsCreated;
public:
	AudioHandler(const AudioHandler&) = delete;
	AudioHandler(AudioHandler&&) = delete;
	AudioHandler& operator=(const AudioHandler&) = delete;
	AudioHandler& operator=(AudioHandler&&) = delete;

	AudioHandler();
	~AudioHandler();

	bool OnCreate(glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _velocity = glm::vec3(0.0f), glm::vec3 _forward = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 _direction = glm::vec3(0.0f, 1.0f, 0.0f));
	void OnDestroy();
	void Update();

	static AudioHandler* GetInstance();
private:
	FMOD_VECTOR ConvertGlmVec3(glm::vec3 _vector);

	void UpdatePositionAndVelocity(int _channel, glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _velocity = glm::vec3(0.0f));

	void LoadSound(const std::string& _filePath, const std::string& _soundName, bool _streamAudio, bool _loopSound = false, bool _isAudio3D = false);
	int PlaySound(const std::string _soundName, glm::vec3 _position, glm::vec3 _velocity = glm::vec3(0.0f), float _volume = 2.0f);

	FMOD::Sound* GetSound(const std::string& _soundName);

	bool IsChannelActive(int _channelID);
};

#endif