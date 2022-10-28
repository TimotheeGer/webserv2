#include "../headers/Client.hpp"

// ************************************************************************** //
//                          CONSTRUCTOR DESTRUCTOR :                          //
// ************************************************************************** //


Client::Client(int client_socket, std::string port) { 
	
	bzero(&_d, sizeof(_d));

	this->_d._fd_client = client_socket;
	this->_d._port = port;
	
	this->_d._request_full = false;
	this->_d._read = false;
	this->_d._read_fail = false;
    this->_d._send = false;
    this->_d._send_fail = false;
	this->_d._read_send_ok = false;
	this->_d._boundary_end.first = false;
	this->_d._limit_GET = true;
	this->_d._limit_POST = true;
	this->_d._limit_DELETE = true;
	this->_d._error_code = 0;
	this->_d.autoindex = 0;
	this->_d._upload.first = false;
	// this->_d._upload.second = "";

	return;
};


Client::~Client(void) { 
	

	return;
};


// ************************************************************************** //
//                             MEMBER FONCTION: Recv	                      //
// ************************************************************************** //


int Client::RecvRequest(std::map<int, std::map<std::string, t_scop> > &map_server) {
    
	std::map<std::string, t_scop> MapConf;

    char buff_request[2048];
	bzero(&buff_request, sizeof(buff_request));
    int ret = 0;
	
	std::cout << C_BOLDMAGENTA << "RECV : Client = FD[" << this->GetFd() << "] PORT[" << this->GetPort() << "]" << C_RESET << std::endl;
	
	if ((ret = recv( this->_d._fd_client , buff_request, 2048, 0)) == 0) //if 0 client deconnecter
	{
		PrintMessageError("RECV", "1");
		this->_d._read_fail = true;
		return (EXIT_SUCCESS);
	}
	else if (ret < 0) // if read < 0 c'est une error
	{
		PrintMessageError("RECV", "2");
		this->_d._read_fail = true;
		return (EXIT_SUCCESS);
	}
	else
	{
		//on rempli un vector de char pour recup tout le binaire
		for (int i = 0; i < ret; i++)
			this->_d._RequestVector.push_back(buff_request[i]);
		this->_d._request_string.append(buff_request, ret);		
		// PrintVectorRequest();
		//voir description sur la fonction dans request.cpp
		if (this->request_r.RequestIsFull(_d) == true)
		{
			this->_d._read = true;
			//mettre une val pour dire que tout es complet pour le moment ou lon va send()
	    	this->request_r.GetElementRequest(_d, map_server);
			std::cout << C_RED << "ERROR 1" << C_RESET << std::endl;
			if ((_d._error_code = open_r.OpenFiles(_d, request_r, map_server[_d._server])) == (EXIT_FAILURE | EXIT_SKIP))
			{
				this->_d._read_fail = true;
				std::cout << C_RED << "ERROR 1.1" << C_RESET << std::endl;
				return (EXIT_SUCCESS);
			}
			std::cout << C_RED << __FUNCTION__ <<"ERROR 2" << C_RESET << std::endl;
	
			if (response_r.MakeResponse(request_r, _d) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			std::cout << C_RED << "ERROR 3" << C_RESET << std::endl;
				
			this->_d._RequestVector.clear();
			this->_d._request_string.clear();
		}
	}
    //print
	// PrintRequestRecv();
    memset(buff_request, '\0', sizeof(buff_request));
	return(EXIT_SUCCESS);
};



//on envoi la reponse avec send si on a bien recu toute la requet biensur
int Client::SendResponse(void) {

	std::cout << C_BOLDMAGENTA << "SEND : Client = FD[" << this->GetFd() << "] PORT[" << this->GetPort() << "]" << C_RESET << std::endl;

	// PrintHttp(this->_d._http);

	if (send(this->_d._fd_client, this->_d._http , this->_d._response.length() + this->_d._content_size, 0) == -1)
	{
		PrintMessageError("SEND", "1");
		this->_d._send_fail = true;
		return(EXIT_FAILURE);
	}
	if (_d._http != NULL)
	{
		free(_d._http);
		_d._http = NULL;
	}
	std::cout << std::endl << C_BOLDGREEN << "--------------------Response Send--------------------" << C_RESET << std::endl << std::endl;
	
	this->_d._send = true;
	this->_d._read_send_ok = true;
	// memset(this->_d._http, 0, this->_d._response.size() + this->_d._content_size);
	return(EXIT_SUCCESS);
};


// ************************************************************************** //
//                             MEMBER FONCTION: Get	                          //
// ************************************************************************** //


int     		Client::GetFd(void) {

    return (this->_d._fd_client);
};

std::string		Client::GetPort(void) {

    return (this->_d._port);
};

bool			Client::GetRequestFull(void) {

    return (this->_d._request_full);
};

bool			Client::GetIsRead(void) {

	return (this->_d._read);
};

bool			Client::GetIsRead_fail(void) {

	return (this->_d._read_fail);
};

bool			Client::GetIsSend(void) {

	return (this->_d._send);
};

bool			Client::GetIsSendFail(void) {

	return (this->_d._send_fail);
};


// ************************************************************************** //
//                             MEMBER FONCTION: Print                         //
// ************************************************************************** //


void			Client::PrintRequestRecv(void) {

   	if (!this->_d._request_string.empty())
	{

        std::cout 	<< C_BOLDGREEN << "--------------------Requete recu--------------------" << C_RESET << std::endl << std::endl;
        std::cout 	<< C_BOLDGREEN << this->_d._request_string << C_RESET << std::endl;
        std::cout 	<<  C_BOLDGREEN << "--------------------Requete recu--------------------" << C_RESET << std::endl << std::endl;
    }
	else
	{
		std::cout 	<< C_BOLDGREEN << "--------------------Requete recu--------------------" << C_RESET << std::endl << std::endl;
        std::cout 	<< C_BOLDRED << "Request is empty " << C_RESET << std::endl;
        std::cout 	<< C_BOLDGREEN << "--------------------Requete recu--------------------" << C_RESET << std::endl << std::endl;
	}
}

void			Client::PrintMessageError(std::string str, std::string error) {

	std::cout << C_BOLDMAGENTA << str << " : Client = FD[" << this->GetFd() << "] PORT[" << this->GetPort() << "]" << C_RESET
			  << C_BOLDRED << "ERROR " << error << C_RESET << std::endl;
}


void			Client::PrintHttp(char* http) {

	std::cout 	<< C_BOLDYELLOW << "--------------------http send--------------------" << C_RESET << std::endl << std::endl;
	std::cout << C_BOLDYELLOW << "http lenght = [" << this->_d._response.length() << "]" << C_RESET << std::endl << std::endl;
	std::cout << C_BOLDYELLOW << "http content.size() = [" << this->_d._content_size << "]" << C_RESET << std::endl << std::endl;
	std::cout 	<< C_BOLDYELLOW << "-------------------------------------------------" << C_RESET << std::endl << std::endl;
	std::cout << C_BOLDYELLOW << http << C_RESET << std::endl;
	std::cout 	<< C_BOLDYELLOW << "--------------------http send--------------------" << C_RESET << std::endl << std::endl;
}

void			Client::PrintVectorRequest() {

	std::cout 	<< C_BOLDYELLOW << "--------------------Vetor Request--------------------" << std::endl << std::endl;

	for (long unsigned int i = 0; i < _d._RequestVector.size(); i++) {

		std::cout << _d._RequestVector[i];
	}
	std::cout << std::endl << C_BOLDYELLOW << "--------------------Vetor Request--------------------" << C_RESET << std::endl << std::endl;
}