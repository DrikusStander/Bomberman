#include "Sound.class.hpp"

//start canonical form
Sound::Sound(void) : volMusic(0.5), volEffects(0.5)
{
	std::cout << "Sound - Default Constructor Called" << std::endl;

	this->isInitialized = false;
	this->Setup();
	this->isMusicPlaying = false;
}

Sound::Sound(Sound const & src)
{
	std::cout << "Sound - Copy constructor called" << std::endl;
	*this = src;
}

Sound::~Sound(void)
{
	std::cout << "Sound - Destructor called" << std::endl;
	if (this->isInitialized == true)
		this->soundEngine->drop();
}

Sound	&Sound::operator=(Sound const &rhs)
{
	std::cout << "Sound - Assignment operator called" << std::endl;
	if (this != &rhs)
	{
		this->soundEngine = rhs.soundEngine;
		this->backgroundMusic = rhs.backgroundMusic;
	}
	return (*this);
}
//end canonical form

void	Sound::Setup(void)
{
	if (this->isInitialized == true)
		this->soundEngine->drop();
	this->soundEngine = irrklang::createIrrKlangDevice();
	if (!this->soundEngine)
		throw Exceptions::InitializeSoundEngineFailed();
	this->isInitialized = true;
	irrklang::ISoundSource *loadMusic = this->soundEngine->addSoundSourceFromFile(MUSIC);
	this->backgroundMusic = this->soundEngine->addSoundSourceAlias(loadMusic, "backgroundMusic");

	irrklang::ISoundSource *loadBombExplode = this->soundEngine->addSoundSourceFromFile(BOMBEXPLODE);
	this->bombExplode = this->soundEngine->addSoundSourceAlias(loadBombExplode, "bombExplode");

	this->backgroundMusic->setDefaultVolume(this->volMusic);
	this->bombExplode->setDefaultVolume(this->volEffects);
}

void	Sound::setVolMusic(const double volume)
{
	if ((volume <= 1.0) && (volume >= 0.0))
	{
		this->volMusic = volume;
		this->Setup();
		if (this->isMusicPlaying == true)
			this->soundEngine->play2D(this->backgroundMusic, true);
	}
}

void	Sound::setVolEffects(const double volume)
{
	if ((volume <= 1.0) && (volume >= 0.0))
	{
		this->volEffects = volume;
		this->Setup();
		if (this->isMusicPlaying == true)
			this->soundEngine->play2D(this->backgroundMusic, true);
	}
}

void	Sound::playMusic(void)
{
	this->isMusicPlaying = true;
	this->soundEngine->play2D(this->backgroundMusic, true);
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
