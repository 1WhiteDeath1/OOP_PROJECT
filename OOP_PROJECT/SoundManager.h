#pragma once
#include <SFML/Audio.hpp>
using namespace sf;

// plays the game's sound effects and background music. everything is static so any class can
// call SoundManager::play(...) without passing an object around. the sounds are loaded once
class SoundManager
{
public:
	enum Effect { SHOOT, ROCKET, EXPLOSION, JUMP, PICKUP, HURT, ENEMY_DEATH, MISSION_COMPLETE, GAME_OVER, EFFECT_COUNT };

	static void play(Effect e, float volume = 100);
	static void playMusic();
	static void stopMusic();
	static void toggleMute();
	static bool isMuted() { return muted; }

private:
	static void load();

	static SoundBuffer buffers[EFFECT_COUNT];
	static bool loaded;
	static bool muted;

	// SFML can only play a sound while its sf::Sound object exists, so a few are kept and reused
	// in turn; that way several sounds (e.g. shots and an explosion) can play at the same time
	static const int VOICES = 16;
	static Sound voices[VOICES];
	static int nextVoice;

	static Music music;
};
