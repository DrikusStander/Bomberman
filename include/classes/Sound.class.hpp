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

# define MUSIC "resources/Sound/media/getout.ogg"
# define BOMBEXPLODE "resources/Sound/media/explosion.wav"

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
	private:
		void	Setup(void);

		irrklang::ISoundEngine	*soundEngine;
		irrklang::ISoundSource	*backgroundMusic;
		irrklang::ISoundSource	*bombExplode;

		bool	isInitialized;
		bool	isMusicPlaying;
		double	volMusic;
		double	volEffects;
};

#endif
