#ifndef		CLIENT_HPP
# define 	CLIENT_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <ostream>
#include <fstream>
#include <map>
#include <vector>
#include "Open.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Client {

	public:

		Client(int client_socket, std::string port);
		~Client(void);


		Open			open_r;
		Request			request_r;
		Response		response_r;

		//main
		int				ClientMakeTaff(std::map<std::string, t_scop> &MapConf);
        
		//recv
        int     		RecvRequest(std::map<int, std::map<std::string, t_scop> > &map_server);
        
		//send
        int             SendResponse(void);
        
		//get
		int     		GetFd(void);
		std::string    	GetPort(void);
		bool			GetRequestFull(void);
		bool			GetIsRead(void);
		bool			GetIsRead_fail(void);
		bool			GetIsSend(void);
		bool			GetIsSendFail(void);
		
		//print
		void			PrintRequestRecv(void);
		void			PrintMessageError(std::string str, std::string error);
		void			PrintHttp(char* http);
		void			PrintVectorRequest(void);
		
	private:

		s_data			_d;
        
};

#endif
