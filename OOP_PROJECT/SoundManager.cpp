#include "SoundManager.h"

SoundBuffer SoundManager::buffers[SoundManager::EFFECT_COUNT];
bool SoundManager::loaded = false;
bool SoundManager::muted = false;
Sound SoundManager::voices[SoundManager::VOICES];
int SoundManager::nextVoice = 0;
Music SoundManager::music;

void SoundManager::load() {
	// same order as the Effect enum
	const char* files[EFFECT_COUNT] = {
		"shoot.wav", "rocket.wav", "explosion.wav", "jump.wav", "pickup.wav",
		"hurt.wav", "enemy_death.wav", "mission_complete.wav", "game_over.wav", "boss.wav"
	};
	for (int i = 0; i < EFFECT_COUNT; i++)
		buffers[i].loadFromFile(std::string("25I-0504_25I-0644_Assets/sounds/") + files[i]);
	loaded = true;
}

void SoundManager::play(Effect e, float volume) {
	if (muted) return;
	if (!loaded) load();
	Sound& s = voices[nextVoice];
	nextVoice = (nextVoice + 1) % VOICES; // next time use the next voice
	s.setBuffer(buffers[e]);
	s.setVolume(volume);
	s.play();
}

void SoundManager::playMusic() {
	if (music.getStatus() != SoundSource::Playing && music.openFromFile("25I-0504_25I-0644_Assets/sounds/music.wav")) {
		music.setLoop(true);
		music.setVolume(35);
		if (!muted) music.play();
	}
}

void SoundManager::stopMusic() {
	music.stop();
}

void SoundManager::toggleMute() {
	muted = !muted;
	if (muted) music.pause();
	else if (music.getStatus() == SoundSource::Paused) music.play();
}
