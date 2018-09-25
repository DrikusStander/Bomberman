#include "Sound.class.hpp"

Sound::Sound(void) : volMusic(0), volEffects(0)
{
	this->isInitialized = false;
	this->Setup();
	this->isMusicPlaying = false;
	this->musicGame = false;
}

Sound::Sound(Sound const & src)
{
	*this = src;
}

Sound::~Sound(void)
{
	if (this->isInitialized == true)
		this->soundEngine->drop();
}

Sound	&Sound::operator=(Sound const &rhs)
{
	if (this != &rhs)
	{
		this->soundEngine = rhs.soundEngine;
		this->backgroundMusicMenu = rhs.backgroundMusicMenu;
		this->bombExplode = rhs.bombExplode;
		this->pickup = rhs.pickup;
		this->die = rhs.die;
		this->isInitialized = rhs.isInitialized;
		this->isMusicPlaying = rhs.isMusicPlaying;
		this->volMusic = rhs.volMusic;
		this->volEffects = rhs.volEffects;
		this->musicGame = rhs.musicGame;
	}
	return (*this);
}

void	Sound::Setup(void)
{
	if (this->isInitialized == true)
		this->soundEngine->drop();
	this->soundEngine = irrklang::createIrrKlangDevice();
	if (!this->soundEngine)
		throw Exceptions::InitializeSoundEngineFailed();
	this->isInitialized = true;
	irrklang::ISoundSource *loadMusicMenu = this->soundEngine->addSoundSourceFromFile(MUSICMENU);
	this->backgroundMusicMenu = this->soundEngine->addSoundSourceAlias(loadMusicMenu, "bgMusicMenu");
	irrklang::ISoundSource *loadMusicGame = this->soundEngine->addSoundSourceFromFile(MUSICGAME);
	this->backgroundMusicGame = this->soundEngine->addSoundSourceAlias(loadMusicGame, "bgMusicGame");
	irrklang::ISoundSource *loadBombExplode = this->soundEngine->addSoundSourceFromFile(BOMBEXPLODE);
	this->bombExplode = this->soundEngine->addSoundSourceAlias(loadBombExplode, "bombExplode");
	irrklang::ISoundSource *loadPickup = this->soundEngine->addSoundSourceFromFile(PICKUP);
	this->pickup = this->soundEngine->addSoundSourceAlias(loadPickup, "pickup");
	irrklang::ISoundSource *loadDie = this->soundEngine->addSoundSourceFromFile(DIE);
	this->die = this->soundEngine->addSoundSourceAlias(loadDie, "die");
	this->backgroundMusicMenu->setDefaultVolume(this->volMusic);
	this->backgroundMusicGame->setDefaultVolume(this->volMusic);
	this->bombExplode->setDefaultVolume(this->volEffects);
	this->pickup->setDefaultVolume(this->volEffects);
	this->die->setDefaultVolume(this->volEffects);
}

void	Sound::setVolMusic(const double volume)
{
	if ((volume <= 1.0) && (volume >= 0.0))
	{
		this->volMusic = volume;
		this->Setup();
		this->playMusic();
	}
}

void	Sound::setVolEffects(const double volume)
{
	if ((volume <= 1.0) && (volume >= 0.0))
	{
		this->volEffects = volume;
		this->Setup();
		this->playMusic();
	}
}

void	Sound::playMusic(void)
{
	this->isMusicPlaying = true;
	if (this->musicGame == true)
		this->soundEngine->play2D(this->backgroundMusicGame, true);
	else
		this->soundEngine->play2D(this->backgroundMusicMenu, true);
}

void	Sound::stopMusic(void)
{
	if (this->isMusicPlaying == true)
	{
		this->isMusicPlaying = false;
		this->Setup();
	}
}

void	Sound::playBombExplode(void)
{
	this->soundEngine->play2D(this->bombExplode);
}

void	Sound::playPickup(void)
{
	this->soundEngine->play2D(this->pickup);
}

void	Sound::playDie(void)
{ 
	this->soundEngine->play2D(this->die);
}

void	Sound::setMusicGame(const bool val)
{
	this->musicGame = val;
	this->Setup();
	this->playMusic();
}
