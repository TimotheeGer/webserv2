#include "../headers/Server.hpp"

// ************************************************************************** //
//                          CONSTRUCTOR DESTRUCTOR :                          //
// ************************************************************************** //


Server::Server() { 
	
	std::cout << C_GREEN << "\n- Launch Server -\n" << C_RESET << std::endl;
	return ;
};

Server::~Server() { 

	return; 
};


// ************************************************************************** //
//                       MEMBER FONCTION: Wait Connection	                  //
// ************************************************************************** //

void	signal_handler(int sign) {

	(void)sign;
	std::cout << std::endl << "Clear Server" << std::endl;
	
}

int 	signals() {

	std::signal(SIGINT, &signal_handler);
	std::signal(SIGPIPE, SIG_IGN);
	return EXIT_SUCCESS;
}
 
 
int Server::WaitConnection(std::map<int, std::map<std::string, t_scop> > &map_server) {

	int run = 1;
	std::cout << C_BOLDYELLOW << "mapserver size = " << map_server.size() << C_RESET << std::endl;
	if (map_server.size() == 0)
	{
		std::cout << "ERROR CONF" << std::endl;
		return (EXIT_FAILURE);
	}
	//init les socket master en bouclan sur les bloc server
	if (InitSocketSocketMasters(map_server) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	std::cout << C_YELLOW << "\n- Launch Server -\n" << C_RESET << std::endl;
	while(run)
	{
		if (signals())
		{
			run = 0;
			ClearServer();
			exit(EXIT_FAILURE);
		}
		FD_ZERO(&_read_sockets);
		FD_ZERO(&_write_sockets);
		//boucle sur les socket master et les SET dans rfds et wfds
		SetFdSet();
		//si il y a quelque chose dans la map de client on SET dans read et write et on apelle select
		if (SelectSocket() == EXIT_FAILURE)
		{
			ClearServer();
			return (EXIT_FAILURE);
		}
		//on ajoute un nouveau client dans _map_client 
		if (SetNewClient() == EXIT_FAILURE)
		{
			ClearServer();
			return (EXIT_FAILURE);
		}
		//maketaff va recv et send les message si tout c bien passer
		if (makeTaff(map_server) == EXIT_FAILURE)
		{
			ClearServer();
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
};


// ************************************************************************** //
//                   MEMBER FONCTION: Make Taff		                          //
// ************************************************************************** //


int     Server::makeTaff(std::map<int, std::map<std::string, t_scop> > &map_server) {

	(void)map_server;
	//on boucle sur les client pour recv les donner du navigateur
	if (AllClientRecv(map_server) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	//on boucle sur les client pour send les reponse si read es true  
	if (AllClientSend() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// on boucle sur les client et on tcheck les cas a close ou fd_clr
	
	ClearClient();
	return (EXIT_SUCCESS);
}


//on boucle sur les client pour recv les donner du navigateur
int		Server::AllClientRecv(std::map<int, std::map<std::string, t_scop> > &map_server) {

	std::map<int, Client *>::iterator		it = this->_mapClient.begin();
	std::map<int, Client *>::const_iterator	ite = this->_mapClient.end();

	//on boucle sur la map de client
	for (; it != ite; it++) {

		if (it->second->GetIsRead() == false)
		{
			if (FD_ISSET(it->second->GetFd(), &_read_sockets))
			{
				//recvrequest va recup la request es tcheck si elle es full si elle es full elle va la traiter
				if (it->second->RecvRequest(map_server) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
		}
	}
	return (EXIT_SUCCESS);
}


int		Server::AllClientSend(void) {

	std::map<int, Client *>::iterator		it = this->_mapClient.begin();
	std::map<int, Client *>::const_iterator	ite = this->_mapClient.end();

	//on boucle sur la map de client
	for (; it != ite; it++) {
		//on check si la request es full si elle ne les pas on quite sinon on envoi le message
		if (it->second->GetIsRead() == true && it->second->GetIsSend() == false)
		{
			if (FD_ISSET(it->second->GetFd(), &_write_sockets))
			{
				if (it->second->SendResponse() == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}	
		}
	}
	return (EXIT_SUCCESS);
}


void Server::ClearClient(void) {

	// std::cout << C_BOLDYELLOW << "CLEAR 1" << C_RESET << std::endl;
	std::map<int, Client *>::iterator		it = this->_mapClient.begin();
	std::map<int, Client *>::const_iterator	ite = this->_mapClient.end();

	std::map<int, Client *>::iterator tmp;
	//on boucle sur la map de client
	for ( ; it != ite; it++) {
		//si tout es ok request bien recu et envoi de la reponse sans problem
		if (it->second->GetIsRead() == true && it->second->GetIsSend() == true)
		{
			FD_CLR(it->second->GetFd(), &_write_sockets);
			FD_CLR(it->second->GetFd(), &_read_sockets);
			close(it->second->GetFd());
			delete it->second;
			// tmp = it;
			// it++;
			this->_mapClient.erase(it);
			break;
			if (this->_mapClient.size() == 0)
			{
				break;		
			}
			std::cout << C_BOLDYELLOW << "TEST CLEAR 1.3" << C_RESET << std::endl;

		}
		else if (it->second->GetIsRead_fail() == true)
		{
			std::cout << C_BOLDYELLOW << "TEST CLEAR 2" << C_RESET << std::endl;
			FD_CLR(it->second->GetFd(), &_write_sockets);
			FD_CLR(it->second->GetFd(), &_read_sockets);
			close(it->second->GetFd());
			delete it->second;
			tmp = it;
			it++;
			this->_mapClient.erase(tmp);
			// this->_mapClient.erase(it++);
			if (this->_mapClient.size() == 0)
				break;		
			std::cout << C_BOLDYELLOW << "TEST CLEAR 2.1" << C_RESET << std::endl;

		}
		// si send fail 
		else if (it->second->GetIsRead() == true && it->second->GetIsSendFail() == true)
		{
			std::cout << C_BOLDYELLOW << "TEST CLEAR 3" << C_RESET << std::endl;
			close(it->second->GetFd());
			FD_CLR(it->second->GetFd(), &_read_sockets);
			delete it->second;
			tmp = it;
			it++;
			this->_mapClient.erase(tmp);
			if (this->_mapClient.size() == 0)
				break;
			std::cout << C_BOLDYELLOW << "TEST CLEAR 3.1" << C_RESET << std::endl;

		}
		//si le fd es juste ok pour read
		else if (it->second->GetIsSend() == true)
		{
			std::cout << C_BOLDYELLOW << "TEST CLEAR 4" << C_RESET << std::endl;
			close(it->second->GetFd());
			FD_CLR(it->second->GetFd(), &_write_sockets);
			delete it->second;
			tmp = it;
			it++;
			this->_mapClient.erase(tmp);
			if (this->_mapClient.size() == 0)
				break;			
			std::cout << C_BOLDYELLOW << "TEST CLEAR 4.1" << C_RESET << std::endl;

		}
	}
	// std::cout << C_BOLDYELLOW << "CLEAR 2" << C_RESET << std::endl;
	return ;
}


// ************************************************************************** //
//                   MEMBER FONCTION: select functions                        //
// ************************************************************************** //


//on envoi socket master dans accept() et il nous renvoi les nouveau client
int Server::NewClientSocket(int socket_master) {
	
	int addr_size = sizeof(sockaddr_in);
	int client_socket;
	
	sockaddr_in client_addr;
	
	if ((client_socket = accept(socket_master, (struct sockaddr *)&client_addr, (socklen_t*)&addr_size)) < 0)
    {
        perror("In accept");
        return(EXIT_FAILURE);
    }
	return (client_socket);
};


//on ajoute un nouveau client dans _map_client 
int	Server::SetNewClient(void) {
	
	std::map<int, std::string>::iterator it = this->_socket_master.begin();
	std::map<int, std::string>::iterator ite = this->_socket_master.end();
	
	for (; it != ite; it++)
	{
		if (FD_ISSET(it->first, &this->_read_sockets))
		{
			int client_socket = 0;
			if ((client_socket = NewClientSocket(it->first)) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			this->_mapClient[client_socket] = new Client(client_socket, it->second);
			
			//print
			PrintNewClient(client_socket);
		}
	}
	return (EXIT_SUCCESS);
}


//on boucle sur la map de socket master et on set dans read et write
void	Server::SetFdSet(void) {

	std::map<int, std::string>::iterator it = this->_socket_master.begin();
	std::map<int, std::string>::iterator ite = this->_socket_master.end();
	
	for (; it != ite; it++)
	{
		FD_SET(it->first, &_read_sockets);
		FD_SET(it->first, &_write_sockets);
	}
	this->_max_fd = this->_socket_master.size();
	// std::cout << C_BOLDMAGENTA << "Max FD = [" << this->_max_fd << "]" << C_RESET << std::endl;
	return;
}


//si il y a quelque chose dans la map de client on SET dans read et write
int		Server::SelectSocket(void) {

	struct timeval	timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	std::map<int, Client *>::iterator		it = this->_mapClient.begin();
	std::map<int, Client *>::const_iterator	ite = this->_mapClient.end();

	//on boucle sur la map de client
	for (; it != ite; it++) {

		int fd = it->first;
		if (fd > 0)
		{
			FD_SET(fd, &_read_sockets);
			FD_SET(fd, &_write_sockets);
		}
		if (fd > this->_max_fd)
			this->_max_fd = fd;
	}

	//problem avec le max de fd dans select si je met tout juste ou +1 sa fonctionne avec grosse val ok 	
	if ((select(this->_max_fd + 10, &_read_sockets, &_write_sockets, NULL, &timeout)) < 0)
	{
		// std::cout << C_RED << "Error select" << C_RESET << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                   MEMBER FONCTION: Sockets functions                       //
// ************************************************************************** //

bool		Server::CheckIsAllready(std::vector<std::string> tab, std::string str) {

	for (size_t i = 0; i < tab.size(); i++) {

		if (tab[i] == str)
			return (true);
	}
	return (false);
}

int		Server::GetPort(std::map<int, std::map<std::string, t_scop> > &_map_server) {

	std::map<int, std::map<std::string, t_scop> >::iterator it = _map_server.begin();
	std::map<int, std::map<std::string, t_scop> >::iterator ite = _map_server.end();

	std::map<std::string, t_scop>::iterator it2;
	std::map<std::string, t_scop>::iterator ite2;

	for ( ; it != ite; it++)
	{
		it2 = it->second.begin();
		ite2 = it->second.end();

		for ( ; it2 != ite2; it2++)
		{
			if (it2->first == "server")
			{
				if (CheckIsAllready(this->_port_tab, it2->second.listen) == false)
					this->_port_tab.push_back(it2->second.listen);
			}
		}
	}

	std::cout << C_BOLDGREEN << "------------------port tab---------------" << C_RESET << std::endl;
	for (size_t i = 0; i < this->_port_tab.size(); i++)
	{
		std::cout << C_BOLDGREEN << "this->_port_tab tab = [" << this->_port_tab[i] << "]" << C_RESET << std::endl;
	}
	std::cout << C_BOLDGREEN << "------------------_port tab---------------" << C_RESET << std::endl;

	return (EXIT_SUCCESS);
}

//boucle sur les bloc server et init un socket_master dans le vector::_socket_master
int Server::InitSocketSocketMasters(std::map<int, std::map<std::string, t_scop> > &_map_server) {
	
	GetPort(_map_server);
	
	for (size_t i = 0; i < this->_port_tab.size(); i++ )
	{
		int socket_master = 0;
		if ((socket_master = InitSocket(atoi(this->_port_tab[i].c_str()))) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		this->_socket_master[socket_master] = this->_port_tab[i];
	}

	//juste du print 
	PrintMasterSocket();
	return (EXIT_SUCCESS);
}


//retourne les socket master
int Server::InitSocket(uint16_t port) {

	int socket_master;

	this->_addrlen = sizeof(this->_address);
	this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
    this->_address.sin_port = htons( port );
	
	memset(this->_address.sin_zero, '\0', sizeof(this->_address.sin_zero));

	std::cout << C_CYAN << "\n- init socket -\n" << C_RESET << std::endl;
	
	int option = 1;
	
	if ((socket_master = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        return(EXIT_FAILURE);
    }
	
	setsockopt(socket_master, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    
	if (bind(socket_master, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
    {
        perror("In bind");
        return(EXIT_FAILURE);
    }
    if (listen(socket_master, 1000) < 0)
    {
        perror("In listen");
        return(EXIT_FAILURE);
    }
	return(socket_master);
};


// ************************************************************************** //
//                   		MEMBER FONCTION: Clear	                          //
// ************************************************************************** //

void Server::ClearServer() {

	std::cout << "Clear server" << std::endl;
	std::map<int, Client *>::iterator itb = this->_mapClient.begin();
	std::map<int, Client *>::iterator ite = this->_mapClient.end();

	for (; itb != ite; itb++)
	{
		FD_CLR(itb->second->GetFd(), &_write_sockets);
		FD_CLR(itb->second->GetFd(), &_read_sockets);
		close(itb->second->GetFd());
		delete itb->second;
		// itb = 
		// this->_mapClient.erase(itb);
		// if (this->_mapClient.size() == 0)
		// 		break;		
	}
	this->_mapClient.clear();
	return ;
};


// ************************************************************************** //
//                   		MEMBER FONCTION: Print	                          //
// ************************************************************************** //

void	Server::PrintMasterSocket(void) {

	std::map<int, std::string>::iterator it2 = _socket_master.begin();
	std::map<int, std::string>::iterator ite2 = _socket_master.end();

	std::cout << C_MAGENTA << "------------------------Socket Master Map ---------------------------" << C_RESET << std::endl;
	for (; it2 != ite2; it2++ )
	{
		std::cout << C_BOLDMAGENTA << "_socket_master = [" << it2->first << "][" << it2->second << "]" C_RESET << std::endl;
	}
	std::cout << C_MAGENTA << "------------------------Socket Master Map ---------------------------" << C_RESET << std::endl;
}

void	Server::PrintNewClient(int client_socket) {

	std::cout << C_MAGENTA << "------------------------New Client Socket ---------------------------" << C_RESET << std::endl;

	std::cout << C_BOLDMAGENTA << "New Client = FD[" << this->_mapClient[client_socket]->GetFd() << "] PORT[" 
					  << this->_mapClient[client_socket]->GetPort() << "]" << C_RESET << std::endl;

	std::cout << C_MAGENTA << "------------------------New Client Socket ---------------------------" << C_RESET << std::endl;
}