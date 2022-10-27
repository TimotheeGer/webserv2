#ifndef		ERROR_HPP
# define 	ERROR_HPP

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

#include "Global.hpp"
#include "Colors.hpp"


class Error {

	public:

		Error(void);
		~Error(void);

		std::string 							GetCodeErrorConvert(int error);
		int 									IsErrorCode(int error);
		void									InitErrorCode(void);
		int 									ErrorPage(int error);
		int										MakeBinErrorPage(int error, s_data &d);
		int 									LoadDefaultErrorPage(int error, s_data &_d);
	
	private:

		std::map<int, std::string>              _map_code;
        char                                    _code_char[9];
        std::string                             _error_page_html;
		
};

#endif