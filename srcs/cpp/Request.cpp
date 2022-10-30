// ************************************************************************** //
//                               	REQUEST :                          	  	  //
// ************************************************************************** //

#include "../headers/Request.hpp"

Request::Request() { return ; };

Request::~Request() { return ; };


// ************************************************************************** //
//                          MEMBER FONCTION: Get Body                         //
// ************************************************************************** //


int 						Request::GetCorrectMethod() {

	if (Get_Method() == "GET" || Get_Method() == "POST" || Get_Method() == "DELETE")
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}


int							Request::GetElementRequest(s_data &d, std::map<int, std::map<std::string, t_scop> > &map_server) {


	this->_AllRequest = d._request_string;

	// PrintHeader(this->_AllRequest);

	if (GetBodyInRequest(d) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (GetAllElement() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	
	// PrintMapRequest();

	std::map<std::string, t_scop> &MapConf = GetMapConf(d, map_server);
	
	TreatmentPath(d);

	ChecklimitExcept(d, MapConf);
	 
	CheckUpload(d, MapConf);

	if (GetCorrectMethod() == EXIT_FAILURE)
	{
		d._error.first = true;
		d._error.second = 400;
	}
	if (HandleMethod(d) == 405)
	{
		d._error.first = true;
		d._error.second = 405;
	}

	if ((CheckBodySizeConf(d, MapConf) == EXIT_FAILURE) || d._upload.first == false) 
		return (EXIT_SUCCESS);
	else
		UploadFile(d);

	this->_AllRequest.clear();
	
	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                          MEMBER FONCTION: Get MapConf	                  //
// ************************************************************************** //

std::string						Request::GetHostForConf(std::string host) {

	std::string test = host.substr(0, host.find_last_of(":"));

	// std::cout << C_BOLDGREEN << "-----------------GetHostForConf---------------" << C_RESET << std::endl;
	// std::cout << C_BOLDGREEN << "host = [" << test << "]" << C_RESET << std::endl; 
	// std::cout << C_BOLDGREEN << "-----------------GetHostForConf---------------" << C_RESET << std::endl;

	return (test);
}

std::map<std::string, t_scop>&	Request::GetMapConf(s_data &d, std::map<int, std::map<std::string, t_scop> > &map_server) {

	std::string port = d._port;
	std::string host = GetHostForConf(this->Get_Host());
	int i = 1;

	std::map<int, std::map<std::string, t_scop> >::iterator ite = map_server.end();

	std::map<std::string, t_scop>::iterator it2;
	std::map<std::string, t_scop>::iterator ite2;

	for (std::map<int, std::map<std::string, t_scop> >::iterator it = map_server.begin() ; it != ite; it++)
	{
		it2 = it->second.begin();
		ite2 = it->second.end();

		for ( ; it2 != ite2; it2++)
		{
			if (it2->first == "server" && it2->second.listen == port)
			{
				for (size_t i = 0; i < it2->second.server_name.size(); i++)
				{
					if (it2->second.server_name[i] == host)
					{
						// std::cout << C_BOLDCYAN << "server = [" << it->first << "]" << C_RESET << std::endl;
						d._server = it->first;
						return (it->second);
					}
				}
			}
		}
		i++;
	}
	for (std::map<int, std::map<std::string, t_scop> >::iterator it = map_server.begin() ; it != ite; it++)
	{
		it2 = it->second.begin();
		ite2 = it->second.end();

		for ( ; it2 != ite2; it2++)
		{
			if (it2->first == "server" && it2->second.listen == port)
			{
				// std::cout << C_BOLDCYAN << "server 2 = [" << it->first << "]" << C_RESET << std::endl;
				d._server = it->first;
				return (it->second);
			}
		}
		i++;
	}
	d._error.first = true;
	d._error.second = 400;
	return (map_server[0]);
}



// ************************************************************************** //
//                          MEMBER FONCTION: Treatement Path                  //
// ************************************************************************** //


int 	Request::TreatmentPath(s_data &d) {

	ClearPath(d);
	GetFirstPath(d);
	return (EXIT_SUCCESS);
}


//on retire tout les slash en trop dans lurl
int		Request::ClearPath(s_data &d) {

	(void)d;
	std::string path = Get_Path();
	std::string::iterator it = path.begin();
	
	for (long unsigned int i = 0; i < path.size(); i++)
	{
		if (path[i] == '/' && path[i + 1] == '/')
		{
			path.erase(it + (i + 1));
			i--;
		}
	}
	if ((path[path.size() - 1] == '/') && (path.size() > 1))
		path.erase(path.size() - 1);

	this->Set_Path(path);

	return (EXIT_SUCCESS);
}

//on recupere juste le premier path
int		Request::GetFirstPath(s_data &d) {

	(void)d;
	long unsigned int pos = 0;
	std::string path = Get_Path();

	if ((pos = path.find("/", 1)) != std::string::npos)
		path.erase(pos);
	this->_FirstPath = path;

	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                          MEMBER FONCTION: Handle                           //
// ************************************************************************** //



int 	Request::HandleMethod(s_data &d) {

	if (d._limit_DELETE == true)
	{
		if (Get_Method() == "DELETE")
		{
			if (Get_Path().find("/upload/") != std::string::npos)
			{
				std::string path = "./srcs/www" + Get_Path(); // add d._root between
				std::remove(path.c_str());
				std::cout << C_BOLDBLUE << "Client: FD[" << d._fd_client << "] PORT[" << d._port << "]" << C_GREEN << " Delete file: " << C_RESET  
					<< Get_Path() << std::endl << std::endl;
			}
		}
	}
	if (d._limit_GET == false)
	{
		if (this->Get_Method() == "GET")
			return (405);
	}
	if (d._limit_POST == false)
	{
		if (this->Get_Method() == "POST")
			return (405);
	}
	if (d._limit_DELETE == false)
	{
		if (this->Get_Method() == "DELETE")
			return (405);
	}
	return EXIT_SUCCESS;
}


// ************************************************************************** //
//                          MEMBER FONCTION: Get Body                         //
// ************************************************************************** //


int		Request::GetBodyInRequest(s_data &d) {

	long unsigned int pos = 0;

	if (this->_AllRequest.empty())
		return (EXIT_FAILURE);

	if ((pos = this->_AllRequest.find("\r\n\r\n") + 4) == this->_AllRequest.size())
		this->_RequestBody = "";
	else
	{
		//on garde uniquement le header de la requete on supprime ce qui ce trouve derriere
		this->_AllRequest = this->_AllRequest.erase(pos, std::string::npos);
		 
		std::vector<char>::iterator it = d._RequestVector.begin();
		std::vector<char>::iterator ite = d._RequestVector.end();
		//on insert dans request body a partir du body qui es la position pos
		this->_RequestBody.insert(_RequestBody.begin(),it + pos, ite);
	}

	// PrintBodyRequest(d);

	if (d._boundary_end.first == true)
	{
		GetHeaderBody(d);
		GetOnlyBody(d);

		ParseBodyRequest();
		// UploadFile();
	}
		
	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                          MEMBER FONCTION: Get Body                         //
// ************************************************************************** //


int				Request::ChecklimitExcept(s_data &_d, std::map<std::string, t_scop> &MapConf) {

	std::map<std::string, t_scop>::iterator it = MapConf.begin();
	std::map<std::string, t_scop>::iterator ite = MapConf.end();

	for ( ; it != ite; it++) {

		if (it->first == "/upload")
		{
			if (it->second.limit_except.size() != 0)
			{
				_d._limit_GET = false;
				_d._limit_POST = false;
				_d._limit_DELETE = false;

				for (long unsigned int i = 0; i < it->second.limit_except.size(); i++)
				{
					if (it->second.limit_except[i] == "GET")
						_d._limit_GET = true;					
					if (it->second.limit_except[i] == "POST")
						_d._limit_POST = true;
					if (it->second.limit_except[i] == "DELETE")
						_d._limit_DELETE = true;
				}
				break;
			}
		}
		if (it->first == this->Get_FirstPath())
		{
			if (it->second.limit_except.size() != 0)
			{
				_d._limit_GET = false;
				_d._limit_POST = false;
				_d._limit_DELETE = false;

				for (long unsigned int i = 0; i < it->second.limit_except.size(); i++)
				{
					if (it->second.limit_except[i] == "GET")
						_d._limit_GET = true;					
					if (it->second.limit_except[i] == "POST")
						_d._limit_POST = true;
					if (it->second.limit_except[i] == "DELETE")
						_d._limit_DELETE = true;
				}
				break;
			}
		}
	}
	// PrintLimitexcept(_d);
	return (EXIT_SUCCESS);
}

int				Request::CheckUpload(s_data &d, std::map<std::string, t_scop> &MapConf) {

	std::map<std::string, t_scop>::iterator it = MapConf.begin();
	std::map<std::string, t_scop>::iterator ite = MapConf.end();
	
	if (MapConf["server"].upload.size() > 0)
	{
		if (!MapConf["server"].upload[0].empty())
		{
			// std::cout << C_BOLDGREEN << "Upload path1 = " << MapConf["server"].upload[1] << C_RESET << std::endl;
			if (MapConf["server"].upload[0] == "off")
			{
				d._upload.first = false;
				d._upload.second = "";				
			}
			else if (MapConf["server"].upload[0] == "on" && !MapConf["server"].upload[1].empty())
			{
				d._upload.first = true;
				d._upload.second = MapConf["server"].upload[1];
			}
		}
	}
	for ( ; it != ite; it++) {

		if (it->first == this->Get_FirstPath())
		{
			if (it->second.upload.size() == 2)
			{
				if (it->second.upload[0] == "on" && !it->second.upload[1].empty())
				{
					d._upload.first = true;
					d._upload.second = it->second.upload[1];
				}
				else if (it->second.upload[0] == "off")
				{
					d._upload.first = false;
					d._upload.second = "";
				}
			}
		}
	}
	return (EXIT_SUCCESS);

}

int		Request::CheckPathUpload(std::string path) {
	
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0)
		return 0;
	// std::cout << C_BOLDRED << S_ISDIR(statbuf.st_mode) << C_RESET << std::endl; 
	return (S_ISDIR(statbuf.st_mode));
}

// ************************************************************************** //
//                          MEMBER FONCTION: Treatement Body                  //
// ************************************************************************** //

//on recupere uniquement le header du body on le met dans This->this->_HeaderBody
int		Request::GetHeaderBody(s_data &d) {

	(void)d;
	long unsigned int pos = 0;
	std::string temp = this->_RequestBody;

	if ((pos = temp.find("\r\n\r\n")) != std::string::npos)
		this->_HeaderBody = temp.erase(pos, std::string::npos);
	
	// PrintHeaderBody();
	
	return (EXIT_SUCCESS);
}

//on recupere uniquement le body sans les header et les boundary
int		Request::GetOnlyBody(s_data &d) {

	long unsigned int pos = 0;
	std::string temp = this->_RequestBody;

	if ((pos = temp.find("\r\n\r\n")) != std::string::npos)
		this->_OnlyBody = temp.erase(0, pos + 4);
	if ((pos = this->_OnlyBody.find(d._boundary)) != std::string::npos)
		this->_OnlyBody = this->_OnlyBody.erase(pos - 4, std::string::npos);

	// PrintOnlyBody();

	return (EXIT_SUCCESS);
}

int		Request::CheckBodySizeConf(s_data &d, std::map<std::string, t_scop> &MapConf) {

	std::map<std::string, t_scop>::iterator it = MapConf.begin();
	std::map<std::string, t_scop>::iterator ite = MapConf.end();
	
	this->GetFirstPath(d);

	if (!MapConf["server"].client_max_body_size.empty())
	{
		if (this->Get_OnlyBody().size() > (size_t)std::atoi(MapConf["server"].client_max_body_size.c_str()) || this->_RequestBody.size() > (size_t)std::atoi(MapConf["server"].client_max_body_size.c_str()))
		{
			d._error.first = true;
			d._error.second = 413;
			return (EXIT_FAILURE);
		}
	}
	for ( ; it != ite; it++) {

		if (it->first == this->Get_FirstPath())
		{
			if (!it->second.client_max_body_size.empty())
			{
				if (this->Get_OnlyBody().size() > (size_t)std::atoi(it->second.client_max_body_size.c_str()) || this->_RequestBody.size() > (size_t)std::atoi(it->second.client_max_body_size.c_str()))
				{
					d._error.first = true;
					d._error.second = 413;
					return (EXIT_FAILURE);
				}
			}
		}
	}
	return (EXIT_SUCCESS);
}

//on cherche des element dans le header du body pour le moment filename
int	Request::ParseBodyRequest(void) {

	std::vector<std::string> tab;
	long unsigned int pos = 0;
	//on split dans un tab 
	if ((Split_tab(this->_HeaderBody, tab, " \r\n")) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	// PrintTabHeaderBody(tab);
	//on boucle sur le tableau de string et on chercher filename pour recup ce quil ya entre parenthese
	for (long unsigned int i = 0; i < tab.size(); i++)
	{
		if ((pos = tab[i].find("filename=")) != std::string::npos)
			this->_FileNameBody = tab[i].substr(pos + 10, (tab[i].size() - (pos + 11)));
	}
	// std::cout << C_BOLDYELLOW << "FILENAME = [" << this->_FileNameBody << "]" << C_RESET << std::endl;
	
	return (EXIT_SUCCESS);
}

//on prend le body qu'on a recup on cree un fichier et on envoi les donner dedans 
int		Request::UploadFile(s_data &d) {

	std::string path = "./srcs/www" + d._upload.second;

	std::string path_test = path;

	path = path + "/" + this->_FileNameBody;

	if (!this->_OnlyBody.empty()) {

		if (CheckPathUpload(path_test) != 1)
		{
			d._error.first = true;
			d._error.second = 500;
			return (EXIT_SUCCESS);
		}

		std::ofstream ofs;

		ofs.open(path.c_str(), std::ofstream::out);
		if(!ofs.is_open())
		{
			path = "./srcs/www/" + this->_FileNameBody;
			ofs.open(path.c_str(), std::ofstream::out);
			// return(EXIT_FAILURE);
		}
		ofs << this->_OnlyBody;
		ofs.close();

		std::cout << C_BOLDBLUE << "Client: FD[" << d._fd_client << "] PORT[" << d._port << "]" << C_GREEN << " Upload file: " << C_RESET  
					<< this->_FileNameBody << " to " << d._upload.second << std::endl << std::endl;
		
	}
	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                     MEMBER FONCTION: Get Element header                    //
// ************************************************************************** //


int		Request::GetAllElement() {

	if (Split_tab(this->_AllRequest, this->_TabRequest, "\r\n") == EXIT_FAILURE)
		return (EXIT_FAILURE);
		
	if (TreatmentFirstLigne() == EXIT_FAILURE)
		return (EXIT_FAILURE);

	if (TreatmentAfterFirstLigne() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	
	return (EXIT_SUCCESS);
}


int		Request::TreatmentFirstLigne(void) {

	long unsigned int pos = 0;
	std::vector<std::string> tab;

	if (this->_TabRequest.empty())
		return(EXIT_FAILURE);

	if (Split_tab(this->_TabRequest[0], tab, " ") == EXIT_FAILURE)
		return (EXIT_FAILURE);

	if (tab.size() > 0)
		this->_MapRequest["Method"] = tab[0];
	if (tab.size() > 1)
		this->_MapRequest["Path"] = tab[1];
	if (tab.size() > 2)
		this->_MapRequest["Server-Protocol"] = tab[2];

	if (this->_MapRequest.count("Path") && (pos = this->_MapRequest["Path"].find("?")) != std::string::npos)
	{
		this->_MapRequest["query-string"] = this->_MapRequest["Path"].substr(pos + 1);
		this->_MapRequest["Path"] = this->_MapRequest["Path"].substr(0, pos);
	}
	tab.clear();
	return (EXIT_SUCCESS);
}

int		Request::TreatmentAfterFirstLigne(void) {

	std::string str;
	std::string first;
	std::string second;

	if (this->_TabRequest.empty())
		return(EXIT_FAILURE);

	for (long unsigned int i = 1; i < this->_TabRequest.size(); i++)
	{
		str = this->_TabRequest[i];
		first = str.substr(0, str.find(": "));
		second = str.substr(str.find(": ") + 2);
		this->_MapRequest[first] = second;
	}
	str.clear();
	first.clear();
	second.clear();
	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                          MEMBER FONCTION : Request is Full                 //
// ************************************************************************** //


//si boundary first es true on met la request vector dans une string pour chercher le boundary de fin 
//si on la trouver on retourne true sa veu dire que la request es complete
bool									Request::RequestIsFull(s_data &d) {
	
	long unsigned int pos = 0;
	std::string request;

	if (GetBoundary(d) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	if (d._boundary_end.first == true)
	{
		for (long unsigned int i = 0; i < d._RequestVector.size(); i++)
			request.push_back(d._RequestVector[i]);
		if ((pos = request.find(d._boundary_end.second)) != std::string::npos)
		{
			// PrintRequestFull(request);
			return (true);
		}
	}
	else
		return (true);
	return (false);
}


//on chercher boundary dans le header si on trouve on on assigne boundary end a true et le boundary de fin 
int		Request::GetBoundary(s_data &d) {

	long unsigned int pos = 0;
	std::string header;
	std::vector<std::string> tab_header;
	std::string boundary;

	for (long unsigned int i = 0; i < d._RequestVector.size(); i++)
		header.push_back(d._RequestVector[i]);
	if ((pos = header.find("\r\n\r\n")) != std::string::npos)
		header.erase(pos, std::string::npos);
	if (Split_tab(header, tab_header, "\r\n") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	for (long unsigned int i = 0; i < tab_header.size(); i++)
	{
		if ((pos = tab_header[i].find("boundary=")) != std::string::npos)
		{
			boundary = tab_header[i].substr(pos + 10);
			d._boundary = boundary;
			boundary = "--" + boundary + "--";
			d._boundary_end.first = true;
			d._boundary_end.second = boundary;
			break;
		}
	}
	// PrintHeader(header);
	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                          MEMBER FONCTION:GET	                              //
// ************************************************************************** //

std::string								Request::Get_AllRequest(void) {

	return (this->_AllRequest);
};

std::map<std::string, std::string>		Request::Get_MapRequest(void) {

	return (this->_MapRequest);
};

std::string								Request::Get_Method(void) {

	return (this->_MapRequest["Method"]);
};

std::string								Request::Get_Path(void) {

	return (this->_MapRequest["Path"]);
};

std::string								Request::Get_Host(void) {

	return (this->_MapRequest["Host"]);
};

std::string								Request::Get_ContentLength(void) {

	return (this->_MapRequest["Content-Length"]);
};

std::string								Request::Get_ContentType(void) {

	return (this->_MapRequest["Content-Type"]);
};

std::string								Request::Get_Accept(void) {

	return (this->_MapRequest["Accept"]);
};

std::string								Request::Get_AccepEncoding(void) {

	return (this->_MapRequest["Accept-Encoding"]);
};

std::string								Request::Get_AcceptLanguage(void) {

	return (this->_MapRequest["Accept-langage"]);
};

std::string								Request::Get_Connection(void) {

	return (this->_MapRequest["Connection"]);
};

std::string								Request::Get_SecFetchDest(void) {

	return (this->_MapRequest["Sec-Fetch-Dest"]);
};

std::string								Request::Get_SecFetchMode(void) {

	return (this->_MapRequest["Sec-Fetch-Mode"]);
};

std::string								Request::Get_SecFetchSite(void) {

	return (this->_MapRequest["Sec-Fetch-Site"]);
};

std::string								Request::Get_SecFetchUser(void) {

	return (this->_MapRequest["Sec-Fetch-User"]);
};

std::string								Request::Get_SecGpc(void) {

	return (this->_MapRequest["Sec-GPC"]);
};

std::string								Request::Get_UpgradeInsecureRequests(void) {

	return (this->_MapRequest["Upgrade-Insecure-Requests"]);
};

std::string								Request::Get_CacheControl(void) {

	return (this->_MapRequest["Cache-Control"]);
};


std::string								Request::Get_UserAgent(void) {

	return (this->_MapRequest["User-Agent"]);
};


std::string								Request::Get_QueryString(void) {

	return (this->_MapRequest["query-string"]);
};

std::string								Request::Get_Extension(void) {

	long unsigned int pos = 0;
	if ((pos = Get_Path().find_last_of(".")) != std::string::npos)
		return (Get_Path().substr(Get_Path().find_last_of(".")));
	return ("");

};

void									Request::Set_Path(std::string set) {

	this->_MapRequest["Path"] = set;
}

std::string								Request::Get_FirstPath(void) {

	return (this->_FirstPath);
}

std::string								Request::Get_OnlyBody(void) {

	return (this->_OnlyBody);
}


void 	Request::ClearRequest(void) {

	this->_MapRequest.clear();
	return ;
}


// ************************************************************************** //
//                          MEMBER FONCTION:utils                             //
// ************************************************************************** //


int							Request::is_sep(std::string sep, char c)
{
	for (long unsigned int i = 0; i < sep.size(); i++)
	{
		if (c == sep[i])
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}


int 	Request::Split_tab(std::string request, std::vector<std::string> &tab, std::string sep) {

	std::string	str;
	bool		indic = false;

	if(request.size() == 0 || sep.size() == 0)
		return (EXIT_FAILURE);
	for (long unsigned int i = 0; i < request.size(); i++)
	{
		if (!is_sep(sep, request[i]))
		{	
			str.push_back(request[i]);
			indic = true;
		}
		else if (indic == true)
		{
			tab.push_back(str);
			str.clear();
			indic = false;
		}
		if (i == (request.size() - 1) && indic == true)
		{
			tab.push_back(str);
			str.clear();
		}
	}
	return (EXIT_SUCCESS);
}


int 					Request::DeleteFirstLine(void) {

	std::string rd_file;
	std::ofstream outfile; 
	std::ofstream outhead; 
	outfile.open("body_php_new", std::ofstream::out);
	outhead.open("header_php", std::ofstream::out);
	std::ifstream input;
	input.open("body_php", std::ifstream::in);
	
	bool on = 0;
	// long unsigned int pos = 0;
	while (getline(input, rd_file)) 
	{
		long unsigned int pos = rd_file.find('\n'); 
		pos = rd_file.find("\r");
		if (on == 0 && pos != 0) {

			outhead << rd_file + "\n";
		}
		if (pos == 0)
		{
			getline(input, rd_file);
			on = 1;
		}
		if (on == 1)
		{
			outfile << rd_file + "\n";
		}
	}
	input.close();
	outfile.close();
	// remove("body_php");
	rename("body_php", "fd_php");
	rename("body_php_new", "body_php");
	return EXIT_SUCCESS;
};


// ************************************************************************** //
//                          MEMBER FONCTION: Print                             //
// ************************************************************************** //


void	Request::PrintBodyRequest(s_data &d) {

	(void)d;
	std::cout << C_BOLDGREEN << "---------------------------PrintBodyRequest----------------------------------" << C_RESET << std::endl;
	
	if (this->_RequestBody.empty())
		std::cout << C_RED << "the body is Empty" << C_RESET << std::endl;
	else
	{
		std::cout << C_GREEN << this->_RequestBody << C_RESET << std::endl;
	}
	std::cout << C_BOLDGREEN<< "---------------------------PrintBodyRequest----------------------------------" << C_RESET << std::endl;

}

void	Request::PrintTab(std::vector<std::string> &tab, std::string str) {

	std::cout << C_BOLDBLUE << "----------------" << str << "---------------------" << C_RESET << std::endl;

	for (long unsigned int i = 0; i < tab.size(); i++)
		std::cout << C_BOLDGREEN << "tab[" << i << "]" << "[" << tab[i] << "]" << C_RESET << std::endl; 

	std::cout << C_BOLDBLUE << "----------------" << str << "---------------------" << C_RESET << std::endl;
}


void	Request::PrintMapRequest(void) {

	long unsigned int i = 0;
	std::map<std::string, std::string>::iterator it;
	std::map<std::string, std::string>::iterator ite;

	it = this->_MapRequest.begin();
	ite = this->_MapRequest.end();

	std::cout << std::endl << C_BOLDWHITE << "--------------------------PrintMapRequest---------------------------------" << C_RESET << std::endl << std::endl;

	for (; it != ite; it++)
	{
		std::cout << C_BOLDCYAN << "[" << i++ << "]mapRequest[" << it->first << "][" << it->second << "]" << C_RESET << std::endl;
	}
	
	std::cout << std::endl << C_BOLDWHITE << "--------------------------PrintMapRequest---------------------------------" << C_RESET << std::endl << std::endl;
}

void	Request::PrintHeader(std::string header) {

	std::cout << C_BOLDWHITE << "--------------------------Print Header---------------------------------" << C_RESET << std::endl;
	std::cout << C_BOLDCYAN << header << C_RESET << std::endl;
	std::cout << C_BOLDWHITE << "--------------------------Print Header---------------------------------" << C_RESET << std::endl;
}

void	Request::PrintRequestFull(std::string request) {

	std::cout << C_BOLDWHITE << "--------------------------Print Request Full---------------------------------" << C_RESET << std::endl;
	std::cout << C_BOLDCYAN << request << C_RESET << std::endl;
	std::cout << C_BOLDWHITE << "--------------------------Print Request Full---------------------------------" << C_RESET << std::endl;
}

void	Request::PrintHeaderBody(void) {

	std::cout << C_BOLDCYAN << "--------------------------Print body header---------------------------------" << C_RESET << std::endl;
	std::cout << C_BOLDCYAN << this->_HeaderBody << C_RESET << std::endl;
	std::cout << C_BOLDCYAN << "--------------------------Print body header---------------------------------" << C_RESET << std::endl;
}

void	Request::PrintOnlyBody(void) {

	std::cout << C_BOLDRED << "--------------------------Print Only Body---------------------------------" << C_RESET << std::endl;
	std::cout << C_BOLDRED << this->_OnlyBody << C_RESET << std::endl;
	std::cout << C_BOLDRED << "--------------------------Print Only Body---------------------------------" << C_RESET << std::endl;
}

void	Request::PrintTabHeaderBody(std::vector<std::string> &tab) {

	std::cout << C_BOLDYELLOW << "------------------------Tab Header-------------------" << C_RESET << std::endl;

	for (long unsigned int i = 0; i < tab.size(); i++)
		std::cout << C_BOLDYELLOW << "tab[" << i << "] = [" << tab[i] << "]" << C_RESET << std::endl;

	std::cout << C_BOLDYELLOW << "------------------------Tab Header-------------------" << C_RESET << std::endl;
	

}