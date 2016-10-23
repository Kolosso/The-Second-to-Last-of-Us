// Local includes:
#include "AudioManager.h"

// Library includes:
#include <iostream>

AudioManager::AudioManager()
: mp_FMODsystem(0)
, mp_FMODchannel(0)
, mp_FMODchannelMusic(0)
, mp_FMODsound_music(nullptr)
, mp_FMODsound_pShoot(0)
, mp_FMODsound_pReload(0)
, mp_FMODsound_pPickup(0)
, mp_FMODsound_pDeath(0)
, mp_FMODsound_zGroan1(0)
, mp_FMODsound_zGroan2(0)
, mp_FMODsound_zAttack(0)
, mp_FMODsound_zDeath(0)
, volume(0.7f)
, volumeMusic(volume / 3)
, muted(false)
{
}

AudioManager::~AudioManager()
{
	mp_FMODsystem->release();
}

bool
AudioManager::Initialise()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&mp_FMODsystem);      // Create the main system object.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = mp_FMODsystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
	// Player Sound Effects
	result = mp_FMODsystem->createSound("assets\\soundFX\\SOUNDEFFECT.wav", FMOD_2D, 0, &mp_FMODsound_pShoot);
	result = mp_FMODsystem->createSound("assets\\soundFX\\SOUNDEFFECT.wav", FMOD_2D, 0, &mp_FMODsound_pReload);
	result = mp_FMODsystem->createSound("assets\\soundFX\\SOUNDEFFECT.wav", FMOD_2D, 0, &mp_FMODsound_pPickup);
	result = mp_FMODsystem->createSound("assets\\soundFX\\SOUNDEFFECT.wav", FMOD_2D, 0, &mp_FMODsound_pDeath);
	// Zombie Sound Effects
	result = mp_FMODsystem->createSound("assets\\soundFX\\SOUNDEFFECT.wav", FMOD_2D, 0, &mp_FMODsound_zGroan1);
	result = mp_FMODsystem->createSound("assets\\soundFX\\SOUNDEFFECT.wav", FMOD_2D, 0, &mp_FMODsound_zGroan2);
	result = mp_FMODsystem->createSound("assets\\soundFX\\SOUNDEFFECT.wav", FMOD_2D, 0, &mp_FMODsound_zAttack);
	result = mp_FMODsystem->createSound("assets\\soundFX\\SOUNDEFFECT.wav", FMOD_2D, 0, &mp_FMODsound_zDeath);

	// Music by Eric Matyas    www.soundimage.org
	result = mp_FMODsystem->createStream("assets\\soundFX\\Monster-Street-Fighters.mp3", FMOD_2D | FMOD_LOOP_NORMAL, 0, &mp_FMODsound_music);

	if (result != FMOD_OK)
	{
		return(false);
	}

	return(true);
}

void
AudioManager::Update()
{
	mp_FMODsystem->update();
}

// Two methods to play the player / zombie sound effects
void
AudioManager::PlaySound(soundEffectsPlayer _SoundEffect)
{
	if (!muted)
	{
		if (_SoundEffect == se_pSHOOT)
		{
			mp_FMODsystem->playSound(mp_FMODsound_pShoot, 0, 0, &mp_FMODchannel);
		}
		if (_SoundEffect == se_pRELOAD)
		{
			mp_FMODsystem->playSound(mp_FMODsound_pReload, 0, 0, &mp_FMODchannel);
		}
		if (_SoundEffect == se_pPICKUP)
		{
			mp_FMODsystem->playSound(mp_FMODsound_pPickup, 0, 0, &mp_FMODchannel);
		}
		if (_SoundEffect == se_pDEATH)
		{
			mp_FMODsystem->playSound(mp_FMODsound_pDeath, 0, 0, &mp_FMODchannel);
		}
	}
	VolumeControl();
}
void
AudioManager::PlaySound(soundEffectsZombie _SoundEffect)
{
	if (!muted)
	{
		if (_SoundEffect == se_zGROAN1)
		{
			mp_FMODsystem->playSound(mp_FMODsound_zGroan1, 0, 0, &mp_FMODchannel);
		}
		if (_SoundEffect == se_zGROAN2)
		{
			mp_FMODsystem->playSound(mp_FMODsound_zGroan2, 0, 0, &mp_FMODchannel);
		}
		if (_SoundEffect == se_zATTACK)
		{
			mp_FMODsystem->playSound(mp_FMODsound_zAttack, 0, 0, &mp_FMODchannel);
		}
		if (_SoundEffect == se_zDEATH)
		{
			mp_FMODsystem->playSound(mp_FMODsound_zDeath, 0, 0, &mp_FMODchannel);
		}
	}
	VolumeControl();
}

void
AudioManager::PlayMusic()
{
	mp_FMODsystem->playSound(mp_FMODsound_music, 0, 0, &mp_FMODchannelMusic);
	mp_FMODchannelMusic->setVolume(volumeMusic);

}

void
AudioManager::VolumeUp()
{
	volume += 0.1f;
	volumeMusic = volume / 3;
	VolumeControl();
}

void
AudioManager::VolumeDown()
{
	volume -= 0.1f;
	volumeMusic = volume / 3;
	VolumeControl();
}

void
AudioManager::VolumeControl()
{
	if (volume <= 0.0f)
	{
		muted = true;
		volume = 0.0f;
		mp_FMODchannel->setMute(muted);
		mp_FMODchannel->setVolume(volume);
		mp_FMODchannelMusic->setMute(muted);
		mp_FMODchannelMusic->setVolume(volumeMusic);
	}
	else
	{
		muted = false;
		if (volume > 1.0f)
		{
			volume = 1.0f;
		}
		mp_FMODchannel->setMute(muted);
		mp_FMODchannel->setVolume(volume);
		mp_FMODchannelMusic->setMute(muted);
		mp_FMODchannelMusic->setVolume(volumeMusic);
	}
}