#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include <string>
#include "Component.h"

class AudioSourceComponent : public Component {
	std::string soundName;
	int channelID;
public:
	AudioSourceComponent(const std::string& _soundFile, const std::string& _soundName, bool _shouldLoop = false, bool _is3D = false, bool _streamAudio = false);
	~AudioSourceComponent();

	virtual bool OnCreate(GameObject* _gameObject) override;
	virtual void Update(float _deltaTime) override;

	void PlaySound();
	bool IsPlaying() const;
};

#endif