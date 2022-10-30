#ifndef CGI_HPP
#define CGI_HPP

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
#include "Request.hpp"

class Request;

class Cgi {

    public:

        Cgi();
        ~Cgi();

        int                                     InitEnvCgi(Request &request_r, std::map<std::string, t_scop> &MapConf);
        int                                     InterpretCgi(Request &request_r, std::map<std::string, t_scop> &MapConf);
        void                                    ClearArray(char **tab);
        void                                    ClearEnvCgi(void);
        int									    ClearCgi(int error);

    private:


        // int                                     pip[2];
        // pid_t                                   pid;

        int                                     _tmp_file;
        // int                                     tmp_file_error;

        // char                                  tab_arg;
        char**                                  _env_cgi;
        // std::vector<char *>                     _env_cgi;
        std::vector<char *>                     _env_cgi_two;

        std::map<std::string, std::string>      _map_cgi_env;
        std::string                             _req_body;
        std::map<std::string, std::string>      maprequest;

        std::string                             path;
        std::string                             path_chdir;
};

#endif