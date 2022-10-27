// ************************************************************************** //
//                               	 CGI :                           	  	  //
// ************************************************************************** //

/**
 *		TODO	Correct values in environment
 *		TODO 	Variables for values
 *		!		Change names variables and functions
*/

#include "../headers/Cgi.hpp"

Cgi::Cgi(void) { return ; };

Cgi::~Cgi(void) { return ; };

int Cgi::InitEnvCgi(Request &request_r, std::map<std::string, t_scop> &MapConf) {

	_req_body = request_r.Get_AllRequest();
	if (request_r.Get_Method() == "POST")
	{
		_map_cgi_env["CONTENT_TYPE"] = request_r.Get_ContentType();
		_map_cgi_env["CONTENT_LENGTH"] = request_r.Get_ContentLength();
	}

	_map_cgi_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_map_cgi_env["PATH_INFO"] = request_r.Get_Path();
	_map_cgi_env["PATH_TRANSLATED"] = request_r.Get_Path();
	_map_cgi_env["QUERY_STRING"] = request_r.Get_QueryString();
	// std::cout << "FINALQUERY [" << _map_cgi_env["QUERY_STRING"] << "]" << std::endl;
	// _map_cgi_env["REMOTE_ADDR"] = "89.156.110.160";

	// _map_cgi_env["AUTH_TYPE"] = "Basic";
	// _map_cgi_env["REMOTE_IDENT"] = 
	// _map_cgi_env["REMOTE_USER"] = 
	
	_map_cgi_env["REQUEST_METHOD"] = request_r.Get_Method();
	_map_cgi_env["REQUEST_URI"] = request_r.Get_Path(); 

	_map_cgi_env["SCRIPT_NAME"] = "./cgi-bin/php-cgi";
	// _map_cgi_env["SCRIPT_NAME"] = "/usr/bin/php-cgi";		 	// Path .conf

	// _map_cgi_env["SERVER_NAME"] = MapConf["server"].server_name[0];
	_map_cgi_env["SERVER_NAME"] = "localhost";
	_map_cgi_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_map_cgi_env["SERVER_PORT"] = MapConf["server"].listen;						// .conf
	_map_cgi_env["SERVER_SOFTWARE"] = "WEBSERV/1.0";


	_map_cgi_env["REDIRECT_STATUS"] = "200";

	_map_cgi_env["HTTP_ACCEPT"] = request_r.Get_Accept();;
	_map_cgi_env["HTTP_ACCEPT_ENCODING"] = request_r.Get_AccepEncoding();
	_map_cgi_env["HTTP_ACCEPT_LANGUAGE"] = request_r.Get_AcceptLanguage();
	_map_cgi_env["HTTP_CACHE_CONTROL"] = request_r.Get_CacheControl();

	_map_cgi_env["HTTP_CONNECTION"] = request_r.Get_Connection();
	_map_cgi_env["HTTP_HOST"] = request_r.Get_Host();							// . conf
	_map_cgi_env["HTTP_SEC_FETCH_DEST"] = request_r.Get_SecFetchDest();			
	_map_cgi_env["HTTP_SEC_FETCH_MODE"] = request_r.Get_SecFetchMode();
	_map_cgi_env["HTTP_SEC_FETCH_SITE"] = request_r.Get_SecFetchSite();
	_map_cgi_env["HTTP_SEC_FETCH_USER"] = request_r.Get_SecFetchUser();
	_map_cgi_env["HTTP_SEC_GPC"] = request_r.Get_SecGpc();
	_map_cgi_env["HTTP_UPGRADE_INSECURE_REQUESTS"] =request_r.Get_UpgradeInsecureRequests();
	_map_cgi_env["HTTP_USER_AGENT"] = request_r.Get_UserAgent();


	if (!(_env_cgi = (char **)malloc(sizeof(char *) * (_map_cgi_env.size() + 1))))
		return 500;
	int i = 0;

	for (std::map<std::string, std::string>::iterator it = _map_cgi_env.begin(); it != _map_cgi_env.end(); it++) 
	{
		std::string tmp = it->first + "=" + it->second;
		if (!(_env_cgi[i] = strdup(tmp.c_str())))
      		return 500;
		i++;
	}
	_env_cgi[i] = NULL;

	return (200);
};

// int	Cgi::InterpretCgi(std::string path_get, std::string req_body, std::map<std::string, std::string> _map_request) {
int	Cgi::InterpretCgi(Request &request_r, std::map<std::string, t_scop> &MapConf) {

	// Request _request_r = request_r;

	if (InitEnvCgi(request_r, MapConf) == 500)
	{
		ClearEnvCgi();
	std::cout << "Je suis la 1" << std::endl;

		return (500);
	}
	std::string path_ = "body_php";
	std::cout << "Im here 1" << std::endl;
	char **tab = (char**)malloc(sizeof(char *) * 3);
	tab[0] = strdup("./cgi-bin/php-cgi");								// change linux / mac
	// if (!(tab[0] = strdup("/usr/bin/php-cgi")))
	// {
	// 	ClearEnvCgi();
	// 	ClearArray(tab);
	// 	return (500);								// change linux / mac
	// }
	if (!(tab[1] = strdup(request_r.Get_Path().c_str())))
	{
		ClearEnvCgi();
		ClearArray(tab);
	std::cout << "Je suis la 2" << std::endl;

		return 500;
	}
	std::cout << "Im here 2" << std::endl;

	tab[2] = NULL;

	int pip[2];
	if (pipe(pip) != 0)
	{
		ClearEnvCgi();
		ClearArray(tab);
	std::cout << "Je suis la 3" << std::endl;

		return 500;
	}
	std::cout << "Im here 3" << std::endl;

	std::string path_two = request_r.Get_Path();

	this->_tmp_file = ::open(path_.c_str(), O_CREAT | O_RDWR | O_TRUNC, 00755);

	// int tmp_file_error = ::open("error_php", O_CREAT | O_RDWR | O_TRUNC, 00755);

	pid_t pid = fork();

	if (pid == 0) 
	{
		if (!(chdir(path_two.c_str())))
		{
			ClearEnvCgi();
			ClearArray(tab);
	std::cout << "Je suis la 4" << std::endl;

			return 500;
		}
		close(pip[1]);
		if (dup2(pip[0], 0) == -1)
		{
			ClearEnvCgi();
			ClearArray(tab);
	std::cout << "Je suis la 5" << std::endl;

			return 500;
		}
		if (dup2(this->_tmp_file, 1) == -1)
		{
			ClearEnvCgi();
			ClearArray(tab);
	std::cout << "Je suis la 6" << std::endl;

			return 500;
		}
		// if (dup2(tmp_file_error, 2) == -1)
		// 	return 500;
		close(pip[0]);
		execve(tab[0], tab, _env_cgi);
		exit(1);
	}
	else if (pid > 0)
	{
		close(pip[0]);
		if (request_r._RequestBody.length() && write(pip[1], request_r._RequestBody.c_str(), request_r._RequestBody.length()) <= 0)
		{
			ClearEnvCgi();
			ClearArray(tab);
	std::cout << "Je suis la 7" << std::endl;

			return (500);
		}
		close(pip[1]);

		int status;

		if (waitpid(pid, &status, 0) == -1)
		{
			ClearEnvCgi();
			ClearArray(tab);
	std::cout << "Je suis la 8" << std::endl;

			return 500;
		}
		if (WIFEXITED(status) && WEXITSTATUS(status))
		{
			ClearEnvCgi();
			ClearArray(tab);
			return 502;
		}
	}
	else
	{
		ClearEnvCgi();
		ClearArray(tab);
		return 502;
	}
	ClearEnvCgi();
	ClearArray(tab);
	std::cout << "Je suis la 9" << std::endl;
	std::cout << "Im here 4" << std::endl;

	return (200);
};


void								Cgi::ClearEnvCgi(void){


	if (this->_env_cgi != NULL)
	{
		for (unsigned long int i = 0 ; _env_cgi[i] != NULL ; i ++)
		{
			free(_env_cgi[i]);
		}
	}
	_env_cgi = NULL;
}


void								Cgi::ClearArray(char **tab) {

	if (tab != NULL)
	{
		for (unsigned long int i = 0 ; tab[i] != NULL ; i ++)
		{
			free(tab[i]);
		}
	}
	tab = NULL;		
}
