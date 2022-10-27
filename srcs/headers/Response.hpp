#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <algorithm>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <string>
#include <sstream>
#include <iostream>
#include <ostream>
#include <fstream>
#include <map>
#include <vector>
// #include "Server.hpp"
#include "Error.hpp"
#include "Mimes.hpp"
#include "Global.hpp"
#include "Colors.hpp"
#include "Request.hpp"

class Request;

class Response {


	public:
		
		Response(void);
		~Response(void);

		Error			error_r;
		Mimes			mimes_r;
		// Request			request_r;

		int				MakeResponse(Request &request_r, s_data &_d);
		std::string 	WriteResponse(Request &request_r, s_data &_d);
		std::string		FileIsPhp(void);
		//print
		void			PrintHeaderResponse(std::string etiquette);

	private: 

		std::string     _response;
        // char*           _http;
		// int 			_error_code;
		// char*           _content_bin;				
		// long			_content_size;
		// char			_c_size[11];
		
		// int				autoindex;




};

#endif