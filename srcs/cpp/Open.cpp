#include "../headers/Open.hpp"

// ************************************************************************** //
//                          CONSTRUCTOR DESTRUCTOR :                          //
// ************************************************************************** //


Open::Open(void) { return; };

Open::~Open(void) { return ; };


// ************************************************************************** //
//                             MEMBER FONCTION: Open file	                  //
// ************************************************************************** //


int Open::OpenFiles(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf) {

	//on check si c une requete fav icon
	if (IsFavicon(request_r) == EXIT_SKIP)
		return (EXIT_SKIP);
	//on check les cas derreur
	CheckConf(_d, request_r, MapConf);
	//on open le fichier quon a recu dans le path de la requete 
	std::string path = ChooseFile(request_r.Get_Path(), request_r, MapConf, _d);

	if (_d._error.first == true)
		return (error_r.MakeBinErrorPage(_d._error.second, _d));

	std::cout << C_BOLDGREEN << "OPEN PATH [" << path.c_str() << "]" << C_RESET << std::endl;
	
	std::ifstream input(path.c_str(), std::ios::binary);

	if (IsDirectory(path.c_str()) == 1 && _d.autoindex == 0)
	{
		std::cout << C_BOLDRED << "IS DIRECTORY & AUTOINDEX OFF" << C_RESET << std::endl;
		return (error_r.MakeBinErrorPage(404, _d));			//* Fichier manquant -> 404
	}
	std::cout << C_BOLDRED << "OPEN 1" << C_RESET << std::endl;
	if (IsDirectory(path.c_str()) == 1 && _d.autoindex == 1) //* If autoindex afficher et path es un dossier
	{											
		std::cout << C_BOLDRED << "IS DIRECTORY & AOTOINDEX ON" << C_RESET << std::endl;
		return (GenerateAutoIndex(_d, request_r));
	}
	if (!input.is_open())
	{
		std::cout << "ERROR IS OPEN  " << std::endl;
		return (error_r.MakeBinErrorPage(404, _d));
	}
	//calcul la taille
	input.seekg(0, std::ios::end);
	_d._content_size = input.tellg();
	input.seekg(0, std::ios::beg);
	
	//taille long converti en char*
	std::sprintf(_d._c_size, "%lu", _d._content_size);
	
	std::cout << C_BOLDRED << "OPEN 2" << C_RESET << std::endl;
	//malloc la taille de s'quon a ouvert au dessus
	if (!(_d._content_bin = (char*)malloc(sizeof(char) * (_d._content_size + 1))))
		return (EXIT_FAILURE);
	_d._content_bin[_d._content_size] = '\0';
	//avec squon a ouvert(input) on read  dans content_bin
	input.read(_d._content_bin, _d._content_size);
	//print
	std::cout << C_BOLDRED << "OPEN 3" << C_RESET << std::endl;
	// PrintContentBin(_d);
	if (this->_rewrite == true)
		return (301);
	return (200);
}


// ************************************************************************** //
//                             MEMBER FONCTION: AutoIndex	                  //
// ************************************************************************** //


int				Open::GenerateAutoIndex(s_data &_d, Request &request_r) {

	//Setautoindex va generer un fichier contenan le html
	SetAutoindex(request_r);
	
	std::string path = request_r.Get_Path() + "/" + ".autoindex.html"; 
	std::ifstream input2(path.c_str(), std::ios::binary);
	//open le fichier qui contient le html
	if (!input2.good())
	{
		std::cout << C_BOLDRED << "AUTOINDEX 11" << C_RESET << std::endl;
		return (error_r.MakeBinErrorPage(404, _d));		//* Fichier manquant -> 404
	}
	
	//calcul la taille 
	input2.seekg(0, std::ios::end);
	_d._content_size = input2.tellg();
	input2.seekg(0, std::ios::beg);
	
	//taille long converti en char*
	std::sprintf(_d._c_size, "%lu", _d._content_size);
		std::cout << C_BOLDRED << "AUTOINDEX 4" << C_RESET << std::endl;
	
	//malloc la taille de s'quon a ouvert au dessus
	if (!(_d._content_bin = (char*)malloc(sizeof(char) * _d._content_size + 1)))
		return (EXIT_FAILURE);
	_d._content_bin[_d._content_size] = '\0';
	//avec squon a ouvert(input2) on read  dans content_bin
	input2.read(_d._content_bin, _d._content_size);
	//print
		std::cout << C_BOLDRED << _d._content_bin  << C_RESET << std::endl;
	// PrintContentBin(_d);

	return (200);
}


std::string		Open::ExecuteCommand(std::string cmd, Request &request_r) {

	(void)request_r;
	std::ostringstream oss;

	cmd += " > ";
	cmd += request_r.Get_Path();
	cmd += "/.autoindex.html";
	
	if (!oss) // EXCEPTION A CREER
		std::cerr << C_RED << "Error : can't open osstream" <<  C_RESET << std::endl;
	int status = std::system(cmd.c_str());
	if (WEXITSTATUS(status) != 0)
		std::cerr << C_RED << "Error : can't exec cmd " << cmd <<  C_RESET << std::endl; // EXCEPTION
	oss << std::ifstream(&cmd[3]).rdbuf();
	return oss.str();
}


int Open::SetAutoindex(Request &request_r) {

	std::string path = "";
	std::string path2 = "";

	if (request_r.Get_Path().find("./srcs/www") != std::string::npos)
	{
		path = request_r.Get_Path() + "/";
		path2 = request_r.Get_Path().substr(request_r.Get_Path().find("./srcs/www") + 10);	
	}
	// std::string index = "./cgi-bin/tree \"" + path + "\" -H " + path2 + " -L 1 -T " + path + " --noreport --charset en-US";
	std::string index = "./cgi-bin/tree \"" + path + "\" -H " + path2 + " --noreport --charset en-US";
	std::cout << C_GREEN << index << C_RESET << std::endl;
	std::string launch = ExecuteCommand(index.c_str(), request_r);

	// PrintAutoIndex(index, launch);

	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                             MEMBER FONCTION: Check Conf	                  //
// ************************************************************************** //


int	Open::CheckConf(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf) {

	CheckAutoIndex(_d, request_r, MapConf);
	CheckConfLocation(_d, request_r, MapConf);
	CheckErrorPage(_d, request_r, MapConf);
	CheckConfCgi(_d, request_r, MapConf);

	return (EXIT_SUCCESS);
}



int	Open::CheckConfLocation(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf) {

	bool set = false;
	// bool location = false;
	(void)_d;
	this->_rewrite = false;
	// PrintMapConf(MapConf);
	
	std::map<std::string, t_scop>::iterator it = MapConf.begin();
	std::map<std::string, t_scop>::iterator ite = MapConf.end();

	for ( ; it != ite; it++) {

		if (it->first == request_r.Get_Path())
		{
			std::cout << C_BOLDCYAN << "Get path = " << request_r.Get_Path() << C_RESET << std::endl;	
			if (!it->second.root.empty())
			{	
			std::cout << C_BOLDCYAN << "Get path = " << request_r.Get_Path() << C_RESET << std::endl;
				this->_root = it->second.root;
				if (!it->second.rewrite.empty()){

					this->_rewrite = true;
					this->_root += "/" + it->second.rewrite;
				}
				else if (!it->second.index.empty() && _d.autoindex == 0)
					this->_root += "/" + it->second.index;
				else
				{
					this->_root += request_r.Get_Path();
				}
				set = true;
			}
			else
			{
				if (MapConf["server"].root.empty())
					return (EXIT_FAILURE);
				else
				{
					this->_root = MapConf["server"].root;
					if (!it->second.rewrite.empty()){

						this->_rewrite = true;
						this->_root += "/" + it->second.rewrite;
					}
					else if (!it->second.index.empty())
						this->_root += "/" + it->second.index;
					else
					{
						this->_root += request_r.Get_Path();
					}
				}
				set = true;
			}
		}
	}

	if (set == false)
		this->_root = "./srcs/www" + request_r.Get_Path();
	else
		this->_root = "./srcs/" + this->_root;

	request_r.Set_Path(this->_root);

	std::cout << C_BOLDYELLOW << "ROOT OPEN = [" << this->_root << "]" << C_RESET << std::endl;

	return (EXIT_SUCCESS);
}

int				Open::CheckErrorPage(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf) {
	
	(void)request_r;
	std::map<std::string, t_scop>::iterator it = MapConf.begin();
	std::map<std::string, t_scop>::iterator ite = MapConf.end();
	
	if (MapConf["server"].error_page.size() == 2)
	{
		if (!MapConf["server"].error_page[0].empty() && !MapConf["server"].error_page[1].empty())
		{
			_d._error_page.first = std::atoi(MapConf["server"].error_page[0].c_str());
			_d._error_page.second = MapConf["server"].error_page[1];
		}
	}
	for ( ; it != ite; it++) {

		if (it->first == request_r.Get_FirstPath())
		{
			// std::cout << " cgi size is "<< it->second.cgi.size() << std::endl;
			if (it->second.error_page.size() == 2)
			{
				std::cout << "TEST CHECKCONFCGI 2" << std::endl;
				if (!it->second.error_page[0].empty() && !it->second.error_page[1].empty())
				{
					std::cout << C_BOLDGREEN << "second.error_page[0] = " << it->second.error_page[0] << C_RESET << std::endl;
					std::cout << C_BOLDGREEN << "second.error_page[1] = " << it->second.error_page[1] << C_RESET << std::endl;
					_d._error_page.first = std::atoi(it->second.error_page[0].c_str());
					_d._error_page.second = it->second.error_page[1]; 
				}
			}
		}
	}
	//print pair
	// PrintPairErrorPage(_d._error_page);

	return (EXIT_SUCCESS);
}

int	Open::CheckConfCgi(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf) {

	(void)_d;
	this->_cgi = false;
	// PrintMapConf(MapConf);
	
	std::map<std::string, t_scop>::iterator it = MapConf.begin();
	std::map<std::string, t_scop>::iterator ite = MapConf.end();

	if (MapConf["server"].cgi.size() == 2)
	{
		if (!MapConf["server"].cgi[0].empty() && !MapConf["server"].cgi[1].empty())
		{
			if (MapConf["server"].cgi[0] == ".php" && MapConf["server"].cgi[1] == "php-cgi")
			{
					this->_cgi = true;
					return (EXIT_SUCCESS);
			}
		}
	}
	for ( ; it != ite; it++) {

		if (it->first == request_r.Get_FirstPath())
		{
			std::cout << " cgi size is "<< it->second.cgi.size() << std::endl;
			if (it->second.cgi.size() == 2)
			{
				std::cout << "TEST CHECKCONFCGI 2" << std::endl;
				if (it->second.cgi[0] == ".php" && it->second.cgi[1] == "php-cgi")
				{
					std::cout << C_BOLDGREEN << "second.cgi[0] = " << it->second.cgi[0] << C_RESET << std::endl;
					std::cout << C_BOLDGREEN << "second.cgi[1] = " << it->second.cgi[1] << C_RESET << std::endl;
					this->_cgi = true;
					break; 
				}
			}
		}
	}
	return (EXIT_SUCCESS); 
}

int				Open::CheckAutoIndex(s_data &_d, Request &request_r, std::map<std::string, t_scop> &MapConf) {

	std::map<std::string, t_scop>::iterator it = MapConf.begin();
	std::map<std::string, t_scop>::iterator ite = MapConf.end();

	if (!MapConf["server"].autoindex.empty())
	{
		if (MapConf["server"].autoindex == "on")
		{
			_d.autoindex = 1;
			return (EXIT_SUCCESS);
		}
	}
	for ( ; it != ite; it++) {

		if (it->first == request_r.Get_FirstPath())
		{
			std::cout << C_BOLDCYAN << "Get path = " << request_r.Get_Path() << C_RESET << std::endl;
			// location = true;
			if (!it->second.autoindex.empty())
			{
				std::cout << C_BOLDRED << "AUTOINDEX 1" << C_RESET << std::endl;
				if (it->second.autoindex == "on")
				{
				std::cout << C_BOLDRED << "AUTOINDEX 2" << C_RESET << std::endl;
					_d.autoindex = 1;
				}
				break;
			}
		}
	}
	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                             MEMBER FONCTION: Utils		                  //
// ************************************************************************** //


std::string Open::ChooseFile(std::string path, Request &request_r, std::map<std::string, t_scop> &MapConf, s_data &_d) {

	int error = 0;
	std::cout << C_BOLDCYAN << "CGI path = "<< path << C_RESET << std::endl;

	if (path.find(".php", 0) != std::string::npos)
	{
		//* Interpreter le php avec CGI
		if (this->_cgi == false)
		{
			std::cout << C_BOLDCYAN << "CGI 1" << C_RESET << std::endl;
			_d._error.first = true;
			_d._error.second = 500;
			return ("");			
		}
		if ((error = cgi_r.InterpretCgi(request_r, MapConf)) != 200)
		{
				std::cout << C_BOLDCYAN << "CGI 2" << C_RESET << std::endl;
				_d._error.first = true;
				_d._error.second = error;
				return ("");
		}
		//* Parse du body du fichier php en supprimant les lignes vides
		request_r.DeleteFirstLine();	
		return ("body_php");
	}
	else
		return (path);
}


int Open::IsFavicon(Request &request_r) {

   	if (request_r.Get_Path() == "favicon.ico")
	{
		std::cout << "Favicon fail" << std::endl;
		return (EXIT_SKIP);
	}
    else
        return (EXIT_SUCCESS);
};

int Open::IsDirectory(const char *path) {

	struct stat statbuf;
	std::cout << C_RED << "------------test directory-------------" << C_RESET << std::endl;
	std::cout << C_RED << "PATH is directory ? = " << path << C_RESET << std::endl;
	if (stat(path, &statbuf) != 0)
	{
		std::cout << C_BOLDRED << "Is error" << C_RESET << std::endl; 
		return 0;
	}
	std::cout << C_BOLDRED << S_ISDIR(statbuf.st_mode) << C_RESET << std::endl; 
	std::cout << C_RED << "------------test directory-------------" << C_RESET << std::endl;
	return (S_ISDIR(statbuf.st_mode));
}


// ************************************************************************** //
//                             MEMBER FONCTION: Print		                  //
// ************************************************************************** //


void Open::PrintContentBin(s_data &_d) {

	std::cout 	<< C_YELLOW << "--------------------Content--------------------" << C_RESET << std::endl;
	std::cout 	<< C_YELLOW << 	_d._content_bin << C_RESET << std::endl;
	std::cout 	<< C_YELLOW << "--------------------Content--------------------" << C_RESET << std::endl << std::endl;
}

void Open::PrintAutoIndex(std::string index, std::string launch) {

	std::cout << C_BOLDGREEN << "-------------------------AutoIndex-----------------------" << std::endl;
	std::cout << "INDEX = [" <<  index << "]" << std::endl ;
	std::cout << "LANCH = [" <<  launch << "]" << std::endl ;
	std::cout << "-------------------------AutoIndex-----------------------" << C_RESET << std::endl;
}


void Open::PrintMapConf(std::map<std::string, t_scop> &MapConf) {

		std::map<std::string, t_scop>::iterator it;
		std::map<std::string, t_scop>::iterator ite;

		it = MapConf.begin();
		ite = MapConf.end();

		for (; it != ite; it++)
		{
			std::cout << C_BOLDMAGENTA << "---------------------------------------------------------" << C_RESET << std::endl;
			std::cout << C_BOLDWHITE << "SCOP = [" << it->first << "]" << C_RESET << std::endl;
			if (!it->second.autoindex.empty())
				std::cout << C_BOLDGREEN << "map_server second.autoindex = [" << it->second.autoindex << "]" << C_RESET << std::endl;
			for (long unsigned int i = 0; i < it->second.cgi.size(); i++)
				std::cout << C_BOLDGREEN << "map_server second.cgi = [" << it->second.cgi[i] << "]" << C_RESET << std::endl;
			for (long unsigned int i = 0; i < it->second.error_page.size(); i++)
				std::cout << C_BOLDGREEN << "map_server second.error_page = [" << it->second.error_page[i] << "]" << C_RESET << std::endl;
			if (!it->second.index.empty())
				std::cout << C_BOLDGREEN << "map_server second.index = [" << it->second.index << "]" << C_RESET << std::endl;
			if (!it->second.listen.empty())
				std::cout << C_BOLDGREEN << "map_server second.listen = [" << it->second.listen << "]" << C_RESET << std::endl;
			if (!it->second.root.empty())
				std::cout << C_BOLDGREEN << "map_server second.root = [" << it->second.root << "]" << C_RESET << std::endl;
			for (long unsigned int i = 0; i < it->second.server_name.size(); i++)
				std::cout << C_BOLDGREEN << "map_server second.servername = [" << it->second.server_name[i] << "]" << C_RESET << std::endl;
			for (long unsigned int i = 0; i < it->second.limit_except.size(); i++)
				std::cout << C_BOLDGREEN << "map_server second.limit_except = [" << it->second.limit_except[i] << "]" << C_RESET << std::endl;
			if (!it->second.client_max_body_size.empty())
				std::cout << C_BOLDGREEN << "map_server second.client_max_body_size = [" << it->second.client_max_body_size << "]" << C_RESET << std::endl;
		}
}

void			Open::PrintLimitexcept(s_data &_d) {

	std::cout << C_BOLDCYAN << "--------------------LIMIT EXCEPT-----------------------" << C_RESET << std::endl; 
	std::cout << C_BOLDCYAN << "GET =" << _d._limit_GET << C_RESET << std::endl;
	std::cout << C_BOLDCYAN << "POST =" << _d._limit_POST << C_RESET << std::endl;
	std::cout << C_BOLDCYAN << "DELETE =" << _d._limit_DELETE << C_RESET << std::endl;
	std::cout << C_BOLDCYAN << "--------------------LIMIT EXCEPT-----------------------" << C_RESET << std::endl; 

}

void			PrintPairErrorPage(std::pair<int, std::string> pair) {

	std::cout << C_BOLDGREEN << "--------------------Print Error Pair-----------------------" << C_RESET << std::endl; 
	std::cout << C_BOLDGREEN << "error_page.first = " << pair.first << C_RESET << std::endl;
	std::cout << C_BOLDGREEN << "error_page.second = " << pair.second << C_RESET << std::endl;
	std::cout << C_BOLDGREEN << "--------------------Print Error Pair-----------------------" << C_RESET << std::endl; 

}
	

