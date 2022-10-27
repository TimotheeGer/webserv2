#ifndef SERVER_HPP
#define SERVER_HPP

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
#include "Conf.hpp"
#include "Client.hpp"
#include "Request.hpp"

# define EXIT_SKIP 999

class Client;

class Server {

    public:

        Server();
        ~Server();

		Request						request_r;
		Conf						config_r;

		Client						client_r(int client_socket, std::string port);

        int             			WaitConnection(std::map<int, std::map<std::string, t_scop> > &_map_server);
        //socket
        int                         InitSocketSocketMasters(std::map<int, std::map<std::string, t_scop> > &_map_server);
        bool		                CheckIsAllready(std::vector<std::string> tab, std::string str);
        int		                    GetPort(std::map<int, std::map<std::string, t_scop> > &_map_server);
        int             			InitSocket(uint16_t port);
        int             			NewClientSocket(int socket_master);
        //select
        void	                    SetFdSet(void);
        int 	                    SelectSocket(void);
        int 	                    SetNewClient(void);
        //taff
        int                         makeTaff(std::map<int, std::map<std::string, t_scop> > &map_server);
        int                         AllClientRecv(std::map<int, std::map<std::string, t_scop> > &map_server);
        int                         AllClientSend(void);
        //clear
        void                        ClearClient(void);
        void            			ClearServer();
        //print
        void	                    PrintMasterSocket(void);
        void	                    PrintNewClient(int client_socket);

	private:

        //port
        uint16_t                            _port;
        //socket
        std::vector<std::string>            _port_tab;
        std::map<int, std::string>          _socket_master;
        struct sockaddr_in                  _address;
        int                                 _addrlen;
        //select
        int                                 _max_fd;
        //client
		std::map<int, Client *>		        _mapClient;
		std::map<int, int>			        _test;
        fd_set                              _read_sockets;
        fd_set                              _write_sockets;

};

#endif