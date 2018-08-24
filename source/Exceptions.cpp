/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 16:28:27 by cnolte            #+#    #+#             */
/*   Updated: 2018/08/24 12:22:25 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions.hpp"

const char	*Exceptions::CreateWindowFailed::what(void) const throw()
{
	return ("Failed to create GLFW window.");
}

const char	*Exceptions::InitializeGlewFailed::what(void) const throw()
{
	return ("Failed to initialize GLEW.");
}

// const char	*Exceptions::ArgumentSizeOutOfRange::what(void) const throw()
// {
// 	return ("Input size out of valid range. See \"-help\" for minimum & maximum sizes.");
// }

// const char *Exceptions::LibraryDoesNotExist::what(void) const throw()
// {
// 	std::string ret_msg = "Unable to find library file \"" + _filename + "\".";
// 	return (ret_msg.c_str());
// }

// const char *Exceptions::InvalidLibrary::what(void) const throw()
// {
// 	std::string ret_msg = "\"" + _argument + "\" is not a valid Library.";
// 	return (ret_msg.c_str());
// }

// const char *Exceptions::LibraryFunctionDoesNotExist::what(void) const throw()
// {
// 	std::string ret_msg = "Unable to find library function \"" + _functionName + "\".";
// 	ret_msg = ret_msg + "\nDynamic library error: " + _dlerror + '.';
// 	return (ret_msg.c_str());
// }
