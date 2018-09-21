/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 16:21:59 by cnolte            #+#    #+#             */
/*   Updated: 2018/09/21 15:34:11 by cnolte           ###   ########.fr       */
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

	class TooManyArguments: public std::exception
	{
		public:
			const char	*what(void) const throw();
	};

	class InvalidArguments: public std::exception
	{
		public:
			const char	*what(void) const throw();
	};
};

#endif
