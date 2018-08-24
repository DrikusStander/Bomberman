/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 16:21:59 by cnolte            #+#    #+#             */
/*   Updated: 2018/08/24 12:22:30 by cnolte           ###   ########.fr       */
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

	// class ArgumentSizeOutOfRange: public std::exception
	// {
	// 	public:
	// 		const char	*what(void) const throw();
	// };

	// class LibraryDoesNotExist: public std::exception
	// {
	// 	private:
	// 		std::string _filename;
	// 	public:
	// 		virtual const char* what() const throw();
	// 	LibraryDoesNotExist(const std::string filename)
	// 	{
	// 		_filename = filename;
	// 	}
	// };

	// class InvalidLibrary: public std::exception
	// {
	// 	private:
	// 		std::string _argument;
	// 	public:
	// 		virtual const char* what() const throw();
	// 	InvalidLibrary(const std::string argument)
	// 	{
	// 		_argument = argument;
	// 	}
	// };

	// class LibraryFunctionDoesNotExist: public std::exception
	// {
	// 	private:
	// 		std::string _functionName;
	// 		std::string _dlerror;
	// 	public:
	// 		virtual const char* what() const throw();
	// 	LibraryFunctionDoesNotExist(const std::string functionName, const std::string dlerror)
	// 	{
	// 		_functionName = functionName;
	// 		_dlerror = dlerror;
	// 	}
	// };
};

#endif
