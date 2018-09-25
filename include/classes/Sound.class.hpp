/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sound.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/06/11 11:49:11 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOUND_CLASS_HPP
# define SOUND_CLASS_HPP

# include <iostream>

# include "Exceptions.hpp"

# include <irrKlang.h>

# define MUSICMENU "resources/Sound/media/musicMenu.ogg"
# define MUSICGAME "resources/Sound/media/musicGame.ogg"
# define BOMBEXPLODE "resources/Sound/media/explosion.wav"
# define PICKUP "resources/Sound/media/pickup.ogg"
# define DIE "resources/Sound/media/died.wav"

class Sound
{
	public:
		//start canonical form
		Sound(void);
		Sound(Sound const & src);
		~Sound(void);

		Sound	&operator=(Sound const &rhs);
		//end canonical form

		void	setVolMusic(const double volume);
		void	setVolEffects(const double volume);
		void	playMusic(void);
		void	stopMusic(void);
		void	playBombExplode(void);
		void	playPickup(void);
		void	playDie(void);
		void	setMusicGame(const bool val);
	private:
		void	Setup(void);

		irrklang::ISoundEngine	*soundEngine;
		irrklang::ISoundSource	*backgroundMusicMenu;
		irrklang::ISoundSource	*backgroundMusicGame;
		irrklang::ISoundSource	*bombExplode;
		irrklang::ISoundSource	*pickup;
		irrklang::ISoundSource	*die;

		bool	isInitialized;
		bool	isMusicPlaying;
		double	volMusic;
		double	volEffects;
		bool	musicGame;
};

#endif
