#ifndef GLOBAL_H
#define GLOBAL_H

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


typedef struct  s_data { 
	
	int             	_fd_client;
	int             	_error_code;
	std::string			_port;
	//recv
	std::vector<char>	_RequestVector;
	std::string     	_request_string;
	bool            	_request_full;
	//open
	char*           	_content_bin;				
	long				_content_size;
	char				_c_size[11];
	//response
	std::string     	_response;
	char*           	_http;
    //check
	int					autoindex;
	//bool de verification
    bool             	_read;
    bool             	_read_fail;
    bool             	_send;
    bool             	_send_fail;
	bool				_read_send_ok;
	//boundary
	std::string			_boundary;
	std::pair<bool, std::string> _boundary_end;
	std::pair<bool, int> 	_error;
	std::pair<int, std::string> 	_error_page;
	int					test_code;

	std::string 		filename;


	// int 				_get;
	// int 				_post;
	// int 				_delete;
	//limit_except
	bool				_limit_GET;
	bool				_limit_POST;
	bool				_limit_DELETE;
	bool				_max_body_size;
	std::pair<int, std::string> 	_upload;
	int					_server;
	

}				s_data;

extern s_data _d;

typedef struct scop {
			
	std::string	root;
	std::string index;
	std::string autoindex;
	std::string rewrite;
	std::vector<std::string> limit_except;
	std::string listen;
	std::string client_max_body_size;
	std::vector<std::string> upload;
	std::vector<std::string> error_page;
	std::vector<std::string> cgi;
	std::vector<std::string> server_name;

}				t_scop;


#endif