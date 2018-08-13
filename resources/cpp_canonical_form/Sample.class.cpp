/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sample.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:26:34 by cnolte            #+#    #+#             */
/*   Updated: 2018/08/13 17:26:06 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Sample.class.hpp"

//start canonical form
Sample::Sample(void) : _foo(0)
{
	std::cout << "Default Constructor Called" << std::endl;
	return ;
}

Sample::Sample(int const n) : _foo(n)
{
	std::cout << "Parametric Constructor called" << std::endl;

	return ;
}

Sample::Sample(Sample const & src)
{
	std::cout << "Copy constructor called" << std::endl;
	*this = src;

	return ;
}

Sample::~Sample(void)
{
	std::cout << "Destructor called" << std::endl;
	return ;
}

Sample	&Sample::operator=(Sample const &rhs)
{
	std::cout << "Assignment operator called" << std::endl;
	if (this != &rhs)
		this->_foo = rhs.getfoo();
	return (*this);
}
//end canonical form

std::ostream	&operator<<(std::ostream &out, Sample const &value)
{
	out << value.getfoo();
	return (out);
}

int	Sample::getfoo(void) const
{
	return (this->_foo);
}
