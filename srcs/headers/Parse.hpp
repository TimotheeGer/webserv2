#ifndef PARSE_HPP
#define PARSE_HPP

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

class Parse {

	public:

		Parse();
		~Parse();

		int 									ParseRequest(std::string request, std::map<std::string, std::string> &map_req);
		std::vector<std::string> 				SplitTabs(std::string request, std::string del, std::vector<std::string> &tab);
		std::string 							GetCorrectRequest(std::string word, std::string c, int x);
		std::string 							GetMethodParse(std::vector<std::string> &request);
		int 									DeleteFirstLine(void);
		std::string 							GetMethod(std::map<std::string, std::string> &map_req);


	private:
	
		
		std::vector<std::string> 				_TabRequest;
		std::vector<std::string> 				_TabGet;

};

#endif
