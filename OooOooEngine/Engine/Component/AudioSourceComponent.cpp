#include "AudioSourceComponent.h"
#include "../Audio/AudioHandler.h"
#include "../Rendering/3D/GameObject.h"

AudioSourceComponent::AudioSourceComponent(const std::string& _soundFile, const std::string& _soundName, bool _shouldLoop, bool _is3D, bool _streamAudio) :
	channelID(-1) {
	soundName = _soundName;
	AudioHandler::GetInstance()->LoadSound(_soundFile, soundName, _shouldLoop, _is3D, _streamAudio);
}

AudioSourceComponent::~AudioSourceComponent() {

}

bool AudioSourceComponent::OnCreate(GameObject* _gameObject) {
	SetObject(_gameObject);
	return true;
}

void AudioSourceComponent::Update(float _deltaTime) {
}

void AudioSourceComponent::PlaySound() {
	channelID = AudioHandler::GetInstance()->PlaySound(soundName, GetObject()->GetPosition());
}

bool AudioSourceComponent::IsPlaying() const {
	return AudioHandler::GetInstance()->IsChannelActive(channelID);
}