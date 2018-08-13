/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sample.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/06/11 11:49:11 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SAMPLE_CLASS_HPP
# define SAMPLE_CLASS_HPP

# include <iostream>

class Sample
{
	public:
		//start canonical form
		Sample(void);
		Sample(int const n);
		Sample(Sample const & src);
		~Sample(void);

		Sample	&operator=(Sample const &rhs);
		//end canonical form

		int	getfoo(void) const;
	private:
		int	_foo;
};

std::ostream	&operator<<(std::ostream &out, Sample const &value);

#endif
