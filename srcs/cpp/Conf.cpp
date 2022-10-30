#include "../headers/Conf.hpp"

// ************************************************************************** //
//                          CONSTRUCTOR DESTRUCTOR :                          //
// ************************************************************************** //


Conf::Conf(void) { return; };

Conf::~Conf(void) { return; };



// ************************************************************************** //
//                             MEMBER FONCTION: 	                          //
// ************************************************************************** //



int Conf::WorkConf(int ac, char **av) {

	if (OpenConf(ac, av) == EXIT_FAILURE)
	{
		std::cerr << C_RED << "Error problem opening configuration file" << C_RESET << std::endl;
		return (EXIT_FAILURE);
	}
	if (RemoveComment() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (CheckSyntax() == EXIT_FAILURE)
	{
		std::cerr << C_BOLDRED << "Error syntax configuration file" << C_RESET << std::endl;
		return (EXIT_FAILURE);
	}
	if (SplitServerScop() == EXIT_FAILURE)
	{
		std::cerr << C_BOLDRED << "Error syntax configuration file" << C_RESET << std::endl;
		return (EXIT_FAILURE);
	}
	if (FindLocation() == EXIT_FAILURE)
	{
		std::cerr << C_BOLDRED << "Error syntax configuration file" << C_RESET << std::endl;
		return (EXIT_FAILURE);
	}
	
	MakeServerNameDefault();

	GetPort();

	// PrintValConf();

	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//               MEMBER FONCTION: open conf and return string                 //
// ************************************************************************** //

bool		Conf::CheckIsAllready(std::vector<std::string> tab, std::string str) {

	for (size_t i = 0; i < tab.size(); i++) {

		if (tab[i] == str)
			return (true);
	}
	return (false);
}

int		Conf::GetPort(void) {

	std::map<int, std::map<std::string, t_scop> >::iterator it = this->_map_server.begin();
	std::map<int, std::map<std::string, t_scop> >::iterator ite = this->_map_server.end();

	std::map<std::string, t_scop>::iterator it2;
	std::map<std::string, t_scop>::iterator ite2;


	std::vector<std::string> port;

	for ( ; it != ite; it++)
	{
		it2 = it->second.begin();
		ite2 = it->second.end();

		for ( ; it2 != ite2; it2++)
		{
			if (it2->first == "server")
			{
				if (CheckIsAllready(port, it2->second.listen) == false)
					port.push_back(it2->second.listen);
			}
		}
	}

	// std::cout << C_BOLDGREEN << "------------------port tab---------------" << C_RESET << std::endl;
	// for (size_t i = 0; i < port.size(); i++)
	// {
	// 	std::cout << C_BOLDGREEN << "port tab = [" << port[i] << "]" << C_RESET << std::endl;
	// }
	// std::cout << C_BOLDGREEN << "------------------port tab---------------" << C_RESET << std::endl;

	return (EXIT_SUCCESS);
}



// ************************************************************************** //
//               MEMBER FONCTION: open conf and return string                 //
// ************************************************************************** //


int Conf::OpenConf(int ac, char **av) {

	if (ac != 2)
	{
		std::cout << C_MAGENTA << "Absence of configuration file, the default configuration file will be opened" << C_RESET << std::endl;
		return (Open_and_return_string("./conf/default.conf"));
	}

	std::string av_str = av[1];
	
	if (av_str.find("./conf/") == std::string::npos)
		av_str = "./conf/" + av_str;
	
	if (av_str.substr(av_str.find_last_of(".") + 1) == "conf")
	{
		if (Open_and_return_string(av_str.c_str()) == EXIT_FAILURE)
		{
			std::cout << C_RED << "Error while opening the selected configuration file, the default configuration file will be opened" << C_RESET << std::endl;
			return (Open_and_return_string("./conf/default.conf"));
		}
		std::cout << C_GREEN << "Configuration file : " << av[1] << " is open" << C_RESET << std::endl;
		return (EXIT_SUCCESS);
	}
	else
	{
		std::cout << C_MAGENTA << "Absence of configuration file, the default configuration file will be opened" << C_RESET << std::endl;
		return (Open_and_return_string("./conf/default.conf"));
	}
	return (EXIT_SUCCESS);
};


int Conf::Open_and_return_string(const char * path)
{
	std::ifstream input(path, std::ios::in);
	if (!input.is_open())
		return (EXIT_FAILURE);
	
	input.seekg(0, std::ios::end);
	this->_content_size = input.tellg();
	input.seekg(0, std::ios::beg);
	
	if (!(this->_content_bin = (char*)malloc(sizeof(char) * (this->_content_size + 1))))
		return (EXIT_FAILURE);
	this->_content_bin[this->_content_size] = '\0';

	input.read(this->_content_bin, this->_content_size);
	this->_conf_file = this->_content_bin;
	if (this->_content_bin != NULL)
	{
		free(this->_content_bin);
		this->_content_bin = NULL;
	}
	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//               MEMBER FONCTION: remove comment to string                    //
// ************************************************************************** //


int		Conf::RemoveComment(void) {

	std::vector<std::string> 	tab;
	std::string					str;

	Split_tab(this->_conf_file, tab, "\n");

	for (long unsigned int i = 0; i < tab.size(); i++)
	{
		if (tab[i].find(";") != std::string::npos)
			tab[i] = tab[i].erase(tab[i].find(";") + 1, std::string::npos);
		if (tab[i].find("#") != std::string::npos)
			tab[i] = tab[i].erase(tab[i].find("#"), std::string::npos);
	}
	for (long unsigned int i = 0; i < tab.size(); i++)
	{
			if (!tab[i].empty())
				str.append(tab[i] + "\n");
	}

	this->_conf_file = str;

	// PrintConfFile();

	return (EXIT_SUCCESS);
}

// ************************************************************************** //
//               		MEMBER FONCTION: Check syntax                         //
// ************************************************************************** //


bool		Conf::CheckSpace(std::string str) {

	long unsigned int i = 0;

	if (str.size() == 0)
		return (true);
	while (i < str.size() && (str[i] <= 32))
		i++;
	if (i == str.size())
		return (true);
	return (false);
}

int		Conf::CheckSyntax(void) {

	std::vector<std::string> 	tab;
	std::string					str;

	Split_tab(this->_conf_file, tab, "\n\r");

	// PrintTabConf(tab);

	for (long unsigned int i = 0; i < tab.size(); i++)
	{
		if (CheckSpace(tab[i]) == false)
		{
			if (tab[i].find(";") == std::string::npos && tab[i].find("{") == std::string::npos && tab[i].find("}") == std::string::npos)
					return (EXIT_FAILURE);
			if (tab[i].find(";") != std::string::npos)
			{
				if (tab[i].find("{") != std::string::npos || tab[i].find("}") != std::string::npos)
					return (EXIT_FAILURE);
			}
			if (tab[i].find("{") != std::string::npos || tab[i].find("}") != std::string::npos)
			{
				if (tab[i].find(";") != std::string::npos)
					return (EXIT_FAILURE);
			}
		}
	}
	return (EXIT_SUCCESS);
}



// ************************************************************************** //
//               			MEMBER FONCTION: split server scop                //
// ************************************************************************** //


int		Conf::SplitServerScop(void) {

	std::string tmp;
	int			brakette = 0;
	bool		scop = false;

	for (long unsigned int i = 0; i < this->_conf_file.size(); i++)
	{
		if (this->_conf_file[i] == '{')
		{
			brakette++;
			scop = true;
		}
		if (this->_conf_file[i] == '}')
			brakette--;
		tmp.push_back(this->_conf_file[i]);
		if (brakette == 0 && scop == true)
		{
			_scop_server.push_back(tmp);
			tmp.clear();
			scop = false;
		}
	}
	if (brakette != 0)
		return (EXIT_FAILURE);

	// PrintServerScop();

	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//               				MEMBER FONCTION: get element                  //
// ************************************************************************** //


int Conf::FindLocation(void) {

	std::vector<std::string>		tab;
	std::map<std::string, t_scop> 	map;

	for (long unsigned int i = 0; i < this->_scop_server.size(); i++)
	{
		Split_tab(this->_scop_server[i], tab, " \t\r\n");
		
		if (CheckListen(tab) == false)
			return (EXIT_FAILURE);

		if (GetScopeServer(tab, map) == EXIT_FAILURE)
			return (EXIT_FAILURE);

		if (GetScopeLocation(tab, map) == EXIT_FAILURE)
			return (EXIT_FAILURE);

		if (map["server"].listen.find(";") != std::string::npos)
			map["server"].listen.erase(map["server"].listen.size() - 1);
		this->_map_server[i + 1] = map;
		map.clear();
		tab.clear();
	}
	return (EXIT_SUCCESS);
}

bool	Conf::CheckListen(std::vector<std::string> tab) {

	int nb = 0;

	for (long unsigned int i = 0; i < tab.size(); i++)
	{
		if (tab[i] == "listen")
			nb++;
	}

	if (nb > 1)
		return (false);

	return (true);
}


int		Conf::GetScopeServer(std::vector<std::string> &tab, std::map<std::string, t_scop> &map) {

	std::string	path;
	std::string	tmp;

	for (long unsigned int i = 0; i < tab.size(); i++)
	{
		if (tab[i] == "server" && tab[i + 1] == "{")
		{
			path = tab[i];
			map[path];
			
			while (tab[i] != "location" && (i + 1) < tab.size())
			{
				if (tab[i] == "listen" && (i + 1) < tab.size())
				{
					if (((map[path].listen = tab[i + 1]).find(";") != std::string::npos))
							map[path].listen.erase(map[path].listen.size() - 1);
				}
				if (tab[i] == "root" && (i + 1) < tab.size())
				{
					if ((map[path].root = tab[i + 1]).find(";") != std::string::npos)
						map[path].root.erase(map[path].root.size() - 1);
				}
				if (tab[i] == "client_max_body_size" && (i + 1) < tab.size())
				{
					if ((map[path].client_max_body_size = tab[i + 1]).find(";") != std::string::npos)
						map[path].client_max_body_size.erase(map[path].client_max_body_size.size() - 1);
				}
				if (tab[i] == "cgi" && (i + 1) < tab.size())
				{
					while (tab[i].find(";") == std::string::npos && (i < tab.size()))
					{
						if ((tmp = tab[i + 1]).find(";") != std::string::npos)
							tmp.erase(tmp.size() - 1);
						map[path].cgi.push_back(tmp);
						i++;					
					}
				}
				if (tab[i] == "error_page" && (i + 1) < tab.size())
				{
					while (tab[i].find(";") == std::string::npos && (i < tab.size()))
					{
						if ((tmp = tab[i + 1]).find(";") != std::string::npos)
							tmp.erase(tmp.size() - 1);
						map[path].error_page.push_back(tmp);
						i++;					
					}
				}
				if (tab[i] == "server_name" && (i + 1) < tab.size())
				{
					while (tab[i].find(";") == std::string::npos && (i < tab.size()))
					{
						if ((tmp = tab[i + 1]).find(";") != std::string::npos)
							tmp.erase(tmp.size() - 1);
						map[path].server_name.push_back(tmp);
						i++;					
					}
				}
				if (tab[i] == "upload" && (i + 1) < tab.size())
				{
					while (tab[i].find(";") == std::string::npos && (i < tab.size()))
					{
						if ((tmp = tab[i + 1]).find(";") != std::string::npos)
							tmp.erase(tmp.size() - 1);
						map[path].upload.push_back(tmp);
						i++;					
					}
				}					
				i++;
			}
		}
	}	
	return (EXIT_SUCCESS);
}

int		Conf::GetScopeLocation(std::vector<std::string> &tab, std::map<std::string, t_scop> &map) {

	std::string 	path;
	std::string		tmp;

	bool			accolade = false;

	for (long unsigned int i = 0; i < tab.size(); i++)
	{
		if (tab[i] == "location" && tab[i + 1] == "=" && (i + 2) < tab.size() && accolade == false)
		{
			path = tab[i + 2];
			map[path];
			i += 3;
			accolade = true;
		}
		else if (tab[i] == "location" && (i + 1) < tab.size() && accolade == false)
		{
			path = tab[i + 1];
			map[path];
			i += 2;
			accolade = true;
		}
		if (tab[i] == "{" && accolade == true)
		{
			while(tab[i] != "}" && i + 1 < tab.size())
			{
				if (tab[i] == "root" && (i + 1) < tab.size())
				{
					if ((map[path].root = tab[i + 1]).find(";") != std::string::npos)
						map[path].root.erase(map[path].root.size() - 1);
				}
				if (tab[i] == "index" && (i + 1) < tab.size())
				{
					if ((map[path].index = tab[i + 1]).find(";") != std::string::npos)
						map[path].index.erase(map[path].index.size() - 1);
				}
				if (tab[i] == "rewrite" && (i + 1) < tab.size())
				{
					if ((map[path].rewrite = tab[i + 1]).find(";") != std::string::npos)
						map[path].rewrite.erase(map[path].rewrite.size() - 1);
				}
				if (tab[i] == "autoindex" && (i + 1) < tab.size())
				{
					if ((map[path].autoindex = tab[i + 1]).find(";") != std::string::npos)
						map[path].autoindex.erase(map[path].autoindex.size() -1);
					this->_autoindex = tab[i + 1];
				}
				if (tab[i] == "limit_except" && (i + 1) < tab.size())
				{
					while (tab[i].find(";") == std::string::npos && (i < tab.size()))
					{
						if ((tmp = tab[i + 1]).find(";") != std::string::npos)
							tmp.erase(tmp.size() - 1);
						map[path].limit_except.push_back(tmp);
						i++;					
					}
				}
				if (tab[i] == "cgi" && (i + 1) < tab.size())
				{
					while (tab[i].find(";") == std::string::npos && (i < tab.size()))
					{
						if ((tmp = tab[i + 1]).find(";") != std::string::npos)
							tmp.erase(tmp.size() - 1);
						map[path].cgi.push_back(tmp);
						i++;					
					}
				}
				if (tab[i] == "client_max_body_size" && (i + 1) < tab.size())
				{
					if ((map[path].client_max_body_size = tab[i + 1]).find(";") != std::string::npos)
						map[path].client_max_body_size.erase(map[path].client_max_body_size.size() -1);
				}
				if (tab[i] == "upload" && (i + 1) < tab.size())
				{
					while (tab[i].find(";") == std::string::npos && (i < tab.size()))
					{
						if ((tmp = tab[i + 1]).find(";") != std::string::npos)
							tmp.erase(tmp.size() - 1);
						map[path].upload.push_back(tmp);
						i++;					
					}
				}
				i++;
			}
			accolade = false;
		}
		path.clear();
	}
	return (EXIT_SUCCESS);
}


int		Conf::MakeServerNameDefault(void) {

	std::map<std::string, t_scop>::iterator it;
	std::map<std::string, t_scop>::iterator ite;
	
	std::map<int, std::map<std::string, t_scop> >::iterator it2;
	std::map<int, std::map<std::string, t_scop> >::iterator ite2;


	it2 = _map_server.begin();
	ite2 = _map_server.end();
	
	for (; it2 != ite2; it2++ )
	{
		it = it2->second.begin();
		ite = it2->second.end();

		for (; it != ite; it++)
		{
			if (it->first == "server")
			{
				if (it->second.server_name.size() == 0)
				{
					it->second.server_name.push_back("www.localhost");
					it->second.server_name.push_back("localhost");
				}
			}
		}
	}
	return (EXIT_SUCCESS);	
}



// ************************************************************************** //
//               				MEMBER FONCTION: get element                  //
// ************************************************************************** //

std::map<int, std::map<std::string, t_scop> > &	Conf::GetMapServe(void) {

	return(this->_map_server);
}



// ************************************************************************** //
//                          MEMBER FONCTION:utils                             //
// ************************************************************************** //


int							Conf::is_sep(std::string sep, char c)
{
	for (long unsigned int i = 0; i < sep.size(); i++)
	{
		if (c == sep[i])
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}


int 	Conf::Split_tab(std::string conf, std::vector<std::string> &tab, std::string sep) {

	std::string	str;
	bool		indic = false;

	if(conf.size() == 0 || sep.size() == 0)
		return (EXIT_FAILURE);
	for (long unsigned int i = 0; i < conf.size(); i++)
	{
		if (!is_sep(sep, conf[i]))
		{	
			str.push_back(conf[i]);
			indic = true;
		}
		else if (indic == true)
		{
			tab.push_back(str);
			str.clear();
			indic = false;
		}
		if (i == (conf.size() - 1) && indic == true)
		{
			tab.push_back(str);
			str.clear();
		}
	}
	return (EXIT_SUCCESS);
}


// ************************************************************************** //
//                          MEMBER FONCTION: Print                            //
// ************************************************************************** //


void	Conf::PrintConfFile(void) {

	std::cout << C_BOLDMAGENTA << "---------------------------remove comment to string------------------------------" << C_RESET << std::endl;
	std::cout << C_BOLDBLUE << "conf file = [" << this->_conf_file << "]" << C_RESET << std::endl;
	std::cout << C_BOLDMAGENTA << "---------------------------remove comment to string------------------------------" << C_RESET << std::endl << std::endl;
}

void	Conf::PrintServerScop(void) {
	
	std::cout << C_BOLDMAGENTA << "---------------------------split server scop------------------------------" << C_RESET << std::endl;
	for (long unsigned int i = 0; i < this->_scop_server.size(); i++)
	{
		std::cout << "scop [" << i << "]" << std::endl << "[" << this->_scop_server[i] << "]" << C_RESET << std::endl;
	std::cout << C_BOLDMAGENTA << "---------------------------split server scop------------------------------" << C_RESET << std::endl;
	}
} 


void	Conf::PrintValConf(void) {

	std::map<std::string, t_scop>::iterator it;
	std::map<std::string, t_scop>::iterator ite;
	std::map<int, std::map<std::string, t_scop> >::iterator it2;
	std::map<int, std::map<std::string, t_scop> >::iterator ite2;


	it2 = _map_server.begin();
	ite2 = _map_server.end();
	
	for (; it2 != ite2; it2++ )
	{
		std::cout << C_BOLDRED << "----------------------------------------------------------------------" << C_RESET << std::endl;
		std::cout << C_BOLDRED << "SERVER = [" << it2->first << "]" << C_RESET << std::endl;
		it = it2->second.begin();
		ite = it2->second.end();

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
			if (!it->second.rewrite.empty())
				std::cout << C_BOLDGREEN << "map_server second.rewrite = [" << it->second.rewrite << "]" << C_RESET << std::endl;
			if (!it->second.listen.empty())
				std::cout << C_BOLDGREEN << "map_server second.listen = [" << it->second.listen << "]" << C_RESET << std::endl;
			if (!it->second.root.empty())
				std::cout << C_BOLDGREEN << "map_server second.root = [" << it->second.root << "]" << C_RESET << std::endl;
			for (long unsigned int i = 0; i < it->second.server_name.size(); i++)
				std::cout << C_BOLDGREEN << "map_server second.servername = [" << it->second.server_name[i] << "]" << C_RESET << std::endl;
			for (long unsigned int i = 0; i < it->second.limit_except.size(); i++)
				std::cout << C_BOLDGREEN << "map_server second.limit_except = [" << it->second.limit_except[i] << "]" << C_RESET << std::endl;
			for (long unsigned int i = 0; i < it->second.upload.size(); i++)
				std::cout << C_BOLDGREEN << "map_server second.upload = [" << it->second.upload[i] << "]" << C_RESET << std::endl;
			if (!it->second.client_max_body_size.empty())
				std::cout << C_BOLDGREEN << "map_server second.client_max_body_size = [" << it->second.client_max_body_size << "]" << C_RESET << std::endl;
			

		}
		std::cout << C_BOLDMAGENTA << "---------------------------------------------------------" << C_RESET << std::endl;
		std::cout << C_BOLDRED << "----------------------------------------------------------------------" << C_RESET << std::endl;
	}
	return ;
}

void		Conf::PrintTabConf(std::vector<std::string> &tab) {

	for (unsigned long int i = 0; i < tab.size(); i++)
		std::cout << C_BOLDBLUE << "[" << tab[i] << "]" << C_RESET << std::endl;
}
