#ifndef		OPEN_HPP
# define 	OPEN_HPP

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
#include "Cgi.hpp"
#include "Error.hpp"
#include "Parse.hpp"
#include "Global.hpp"

#define EXIT_SKIP 999


class Request;
class Open {

	public:

		Open(void);
		~Open(void);

		//open
		int 			OpenFiles(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf);
		std::string		ExecuteCommand(std::string cmd, Request &request_r);
		//autoindex
		int				GenerateAutoIndex(s_data &_d, Request &request_r);
		int 			SetAutoindex(Request &request_r);
		int				CheckConf(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf);
		int				CheckConfLocation(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf);
		int				CheckErrorPage(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf);
		int				ChecklimitBodySize(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf);
		int				CheckConfCgi(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf);
		int				CheckAutoIndex(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf);

		
		Cgi				cgi_r;
		Error			error_r;
		
	private:

        std::string 	ChooseFile(std::string path, Request &request_r, std::map<std::string, t_scop> &MapConf, s_data &d);
		int 			IsFavicon(Request &request_r);
		int				IsDirectory(const char *path);
		//print
		void 			PrintContentBin(s_data &_d);
		void			PrintAutoIndex(std::string index, std::string launch);
		void 			PrintMapConf(std::map<std::string, t_scop> &MapConf);
		void			PrintLimitexcept(s_data &_d);
		void			PrintPairErrorPage(std::pair<int, std::string> pair);

		std::string		_root;
		bool			_rewrite;
		bool			_cgi;
};

#endif