#include "AudioHandler.h"

std::map<std::string, FMOD::Sound*> AudioHandler::sounds = std::map<std::string, FMOD::Sound*>();
std::map<int, FMOD::Channel*> AudioHandler::channels = std::map<int, FMOD::Channel*>();

std::unique_ptr<AudioHandler> AudioHandler::instance = nullptr;

AudioHandler::AudioHandler() :
	system(nullptr), channelsCreated(0) {
}

AudioHandler::~AudioHandler() {
	OnDestroy();
}

bool AudioHandler::OnCreate(glm::vec3 _position, glm::vec3 _velocity, glm::vec3 _forward, glm::vec3 _direction) {
	FMOD::System_Create(&system);
	// If there are no audio drivers installed, exit out of the function
	int numDrivers = 0;
	system->getNumDrivers(&numDrivers);
	if (numDrivers == 0) {
		DebugLogger::Warning("No valid audio drivers available! FMOD will not be initialized!", "AudioHandler.cpp", __LINE__);
		return false;
	}
	// Initialize the audio system and return true if all was successful
	if (system->init(10, FMOD_INIT_NORMAL | FMOD_3D | FMOD_INIT_3D_RIGHTHANDED, nullptr) != FMOD_OK) {
		DebugLogger::FatalError("Audio system failed to initialize!", "AudioHandler.cpp", __LINE__);
		return false;
	}
	if (system->set3DListenerAttributes(0, &ConvertGlmVec3(_position), &ConvertGlmVec3(_velocity), &ConvertGlmVec3(_forward), &ConvertGlmVec3(_direction))) {
		DebugLogger::FatalError("Failed to set 3D listener attributes!", "AudioHandler.cpp", __LINE__);
		return false;
	}
	DebugLogger::Info("FMOD successfully initialized!", "AudioHandler.cpp", __LINE__);
	return true;
}

void AudioHandler::OnDestroy() {
	if (sounds.size() > 0) {
		for (auto s : sounds) {
			s.second->release();
			s.second = nullptr;
		}
		sounds.clear();
	}
	if (channels.size() > 0) {
		for (auto c : channels) {
			c.second->stop();
			c.second = nullptr;
		}
		channels.clear();
	}
	system->release();
	system = nullptr;
}

void AudioHandler::Update() {
	if (system) {
		system->update();
	}
}

AudioHandler* AudioHandler::GetInstance() {
	if (instance.get() == nullptr) {
		instance.reset(new AudioHandler);
	}
	return instance.get();
}

FMOD_VECTOR AudioHandler::ConvertGlmVec3(glm::vec3 _vector) {
	FMOD_VECTOR vectorFMOD;
	vectorFMOD.x = _vector.x;
	vectorFMOD.y = _vector.y;
	vectorFMOD.z = _vector.z;
	return vectorFMOD;
}

void AudioHandler::UpdatePositionAndVelocity(int _channelID, glm::vec3 _position, glm::vec3 _velocity) {
	auto it = channels.find(_channelID);
	if (it == channels.end()) {
		return; // Invalid channel ID, don't update position and velocity
	}

	FMOD_VECTOR fPosition = ConvertGlmVec3(_position);
	FMOD_VECTOR fVelocity = ConvertGlmVec3(_velocity);
	if (it->second->set3DAttributes(&fPosition, &fVelocity) != FMOD_OK) {
		DebugLogger::Error("Failed to set channel " + std::to_string(_channelID) + "'s 3D attributes!", "AudioHandler.cpp", __LINE__);
	}
}

void AudioHandler::LoadSound(const std::string& _filePath, const std::string& _soundName, bool _streamAudio, bool _loopSound, bool _isAudio3D) {
	if (GetSound(_soundName) != nullptr) {
		DebugLogger::Warning("Sound with the name \"" + _soundName + "\" already loaded. So, this sound will not be loaded.", "AudioHandler.cpp", __LINE__);
		return;
	}

	FMOD_MODE mode = FMOD_DEFAULT;
	if (_loopSound)		{ mode |= FMOD_LOOP_NORMAL; }
	else				{ mode |= FMOD_LOOP_OFF; }
	if (_isAudio3D)		{ mode |= FMOD_3D; }
	else				{ mode != FMOD_2D; }
	if (_streamAudio)	{ mode |= FMOD_CREATESTREAM; }
	else				{ mode |= FMOD_CREATECOMPRESSEDSAMPLE; }

	FMOD::Sound* sound = nullptr;
	if (system->createSound(_filePath.c_str(), mode, nullptr, &sound) != FMOD_OK) {
		DebugLogger::Error("Failed to load in sound file found in path: \"" + _filePath + "\"", "AudioHandler.cpp", __LINE__);
		return;
	}

	DebugLogger::Info("The audio file \"" + _soundName + "\" found at path \"" + _filePath + "\" has been created!", "AudioHandler.cpp", __LINE__);
	sounds.insert(std::make_pair(_soundName, sound));
}

int AudioHandler::PlaySound(const std::string _soundName, glm::vec3 _position, glm::vec3 _velocity, float _volume) {
	FMOD::Sound* sound = GetSound(_soundName);
	if (!sound) { // The sound doesn't exist, return an error code
		return -1;
	}

	FMOD::Channel* channel = nullptr;
	if (system->playSound(sound, nullptr, true, &channel) != FMOD_OK) {
		DebugLogger::Error("Failed to properly play sound!", "AudioHandler.cpp", __LINE__);
		return -1;
	}

	FMOD_MODE mode;
	FMOD_RESULT result = sound->getMode(&mode);
	if (result == FMOD_OK && mode & FMOD_3D) {
		channel->set3DAttributes(&ConvertGlmVec3(_position), &ConvertGlmVec3(_velocity));
	} else {
		DebugLogger::Error("Failed to get mode!", "AudioHandler.cpp", __LINE__);
		return -1;
	}

	if (channel->setVolume(_volume) != FMOD_OK) {
		DebugLogger::Error("Failed to set the sound's volume!", "AudioHandler.cpp", __LINE__);
		return -1;
	}

	if (channel->setPaused(false) != FMOD_OK) {
		DebugLogger::Error("", "AudioHandler.cpp", __LINE__);
		return -1;
	}

	int id = channelsCreated++;
	channels.insert(std::make_pair(id, channel));
	return id;
}

FMOD::Sound* AudioHandler::GetSound(const std::string& _soundName) {
	auto sound = sounds.find(_soundName);
	if (sound != sounds.end()) {
		return sound->second;
	}
	return nullptr;
}

bool AudioHandler::IsChannelActive(int _channelID) {
	auto it = channels.find(_channelID);
	if (it == channels.end()) {
		return false; // Invalid channel ID, don't update position and velocity
	}

	bool isPlaying;
	if (it->second->isPlaying(&isPlaying) != FMOD_OK) {
		DebugLogger::Error("Failed to find out if channel " + std::to_string(_channelID) + " is playing audio of not.", "AudioHandler.cpp", __LINE__);
	}

	return isPlaying;
}