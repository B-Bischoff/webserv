/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseHeader.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tarchimb <tarchimb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 17:05:50 by tarchimb          #+#    #+#             */
/*   Updated: 2022/08/19 18:28:19 by tarchimb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HEADER_HPP
# define RESPONSE_HEADER_HPP

#include <iostream>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <string>
#include <signal.h>

class ResponseHeader
{
	private:
		std::string	protocol;
		std::string	method;
		std::string	content_type;
		std::string	content_length;
		std::string	get_extension_file(std::string path);
		
	public:
		std::string	response_header;
		int		content_size;
		ResponseHeader();
		~ResponseHeader();
		void	build_response(std::string path);
};

#endif