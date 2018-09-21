/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 16:21:59 by cnolte            #+#    #+#             */
/*   Updated: 2018/09/20 20:09:31 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include <iostream>

namespace Exceptions
{
	class CreateWindowFailed: public std::exception
	{
		public:
			const char	*what(void) const throw();
	};

	class InitializeGlewFailed: public std::exception
	{
		public:
			const char	*what(void) const throw();
	};

	class InitializeSoundEngineFailed: public std::exception
	{
		public:
			const char	*what(void) const throw();
	};
};

#endif
