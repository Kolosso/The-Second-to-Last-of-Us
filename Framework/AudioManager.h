#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

typedef enum
{
	se_pSHOOT, se_pRELOAD, se_pPICKUP, se_pDEATH
}soundEffectsPlayer;

typedef enum
{
	se_zGROAN1, se_zGROAN2, se_zATTACK, se_zDEATH
}soundEffectsZombie;

class AudioManager
{
	// Member Methods
public:
	AudioManager();
	~AudioManager();

	bool Initialise();

	void Update();
	void PlaySound(soundEffectsPlayer _SoundEffectPlayer);
	void PlaySound(soundEffectsZombie _SoundEffectZombie);
	void PlayMusic();

	void VolumeUp();
	void VolumeDown();

protected:
private:
	void VolumeControl();

	// Member Variables
public:
protected:
	FMOD::System* mp_FMODsystem;
	FMOD::Channel* mp_FMODchannel;
	FMOD::Channel* mp_FMODchannelMusic;
	// Other
	FMOD::Sound* mp_FMODsound_music;
	// Player
	FMOD::Sound* mp_FMODsound_pShoot;
	FMOD::Sound* mp_FMODsound_pReload;
	FMOD::Sound* mp_FMODsound_pPickup;
	FMOD::Sound* mp_FMODsound_pDeath;
	// Zombie
	FMOD::Sound* mp_FMODsound_zGroan1;
	FMOD::Sound* mp_FMODsound_zGroan2;
	FMOD::Sound* mp_FMODsound_zAttack;
	FMOD::Sound* mp_FMODsound_zDeath;

private:
	float volume;
	float volumeMusic;
	bool  muted;

};

#endif __AUDIOMANAGER_H__