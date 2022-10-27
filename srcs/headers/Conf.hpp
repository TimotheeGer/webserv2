#ifndef		CONF_HPP
# define 	CONF_HPP

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
#include "Colors.hpp"
#include "Global.hpp"

class Conf {

	public:

		Conf(void);
		~Conf(void);

		int	WorkConf(int ac, char **av);

	private:
		
		
		long															_content_size;
		char*															_content_bin;
		std::string 													_conf_file;

		std::string														_autoindex;
		

	public:


		std::map<int, std::map<std::string, t_scop> > & 		GetMapServe(void);
		std::vector<std::string>										_scop_server;
		std::map<int, std::map<std::string, t_scop> > 			_map_server;
		//* ex: _map_server["8080"]["/data"].root

	private:

		int										OpenConf(int ac, char **av);
		int 									RecupConf(void);
		int 									Open_and_return_string(const char * path);
		int 									RemoveComment(void);
		int										CheckSyntax(void);
		bool									CheckSpace(std::string str);
		bool									CheckListen(std::vector<std::string> tab);
		int										SplitServerScop(void);
		int 									FindLocation(void);
		int										GetScopeServer(std::vector<std::string> &tab, std::map<std::string, t_scop> &map);
		int										GetScopeLocation(std::vector<std::string> &tab, std::map<std::string, t_scop> &map);
		int										GetPort(void);
		bool									CheckIsAllready(std::vector<std::string> tab, std::string str);
		int										MakeServerNameDefault(void);
		//utils
		int										Split_tab(std::string conf, std::vector<std::string> &tab, std::string sep);
		int										is_sep(std::string sep, char c);
		//print
		void									PrintValConf(void);
		void									PrintConfFile(void);
		void									PrintServerScop(void);
		void									PrintTabConf(std::vector<std::string> &tab);


};

#endif