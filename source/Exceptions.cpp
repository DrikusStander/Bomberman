/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 16:28:27 by cnolte            #+#    #+#             */
/*   Updated: 2018/09/21 15:33:56 by cnolte           ###   ########.fr       */
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

const char	*Exceptions::InitializeSoundEngineFailed::what(void) const throw()
{
	return ("Failed to initialize Sound engine.");
}

const char	*Exceptions::TooManyArguments::what(void) const throw()
{
	return ("Failed to start. Too many arguments. Use -help flag for usage.");
}

const char	*Exceptions::InvalidArguments::what(void) const throw()
{
	return ("Failed to start. Invalid arguments. Use -help flag for usage.");
}
