#include "../headers/Error.hpp"

// ************************************************************************** //
//                          CONSTRUCTOR DESTRUCTOR :                          //
// ************************************************************************** //

Error::Error(void) { 

	this->InitErrorCode();	
	return; 
};

Error::~Error(void) { return; };

// ************************************************************************** //
//                             MEMBER FONCTION: Open file	                  //
// ************************************************************************** //


std::string Error::GetCodeErrorConvert(int error) {

	if (this->_map_code.count(error))
		return (this->_map_code[error]);
	return (this->_map_code[400]);
};


int 	Error::IsErrorCode(int error) {

	if (this->_map_code.count(error))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
};

void	Error::InitErrorCode(void) {

	// 100
	this->_map_code[100] = "100 Continue";

	// 200
	this->_map_code[200] = "200 OK";
	this->_map_code[201] = "201 Created";
	this->_map_code[202] = "202 Accepted";
	
	// 300
	this->_map_code[301] = "301 Moved Permanently";						// not sure
	
	// 400
	this->_map_code[400] = "400 Bad Request";						// mauvaise request
	this->_map_code[403] = "403 Forbidden";							
	this->_map_code[404] = "404 Not Found";							// page existe pas
	this->_map_code[405] = "405 Method Not Allowed";				// Get / delete / post
	this->_map_code[408] = "408 Request Time-out";						//* Not sure
	this->_map_code[411] = "411 Length Required";							//* Not sure
	this->_map_code[413] = "413 Request Entity Too Large";
	this->_map_code[414] = "414 Request-URI Too Long";					//* Not sure
	this->_map_code[415] = "415 Unsupported Media Type";
	this->_map_code[429] = "429 Too Many Requests";						//* Not sure
	this->_map_code[451] = "451 Unavailable For Legal Reason";			//* Not sure

	// 500
	this->_map_code[500] = "500 Internal Server Error";
	this->_map_code[501] = "501 Not Implemented";
	this->_map_code[502] = "502 Bad Gateway";

	this->_map_code[503] = "503 Service Unavailable";						//* Not sure
	this->_map_code[504] = "504 Gateway Time-out";						//* Not sure
	this->_map_code[505] = "505 HTTP Version not supported";
};


int Error::ErrorPage(int error) {

    this->_error_page_html += "<!DOCTYPE html>\n\
<html>\n\
	<head>\n\
		<style>\n\
			@import url(\"https://fonts.googleapis.com/css2?family=Didact+Gothic&family=Hind:wght@400;700&display=swap\");\n\
			body {\n\
  				width: 90%;\n\
  				margin: 0 auto;\n\
  				background-color: #d2e9f5;\n\
			}\n\
			h1 {\n\
			  font-family: \"Hind\", sans-serif;\n\
			}\n\
			.img {\n\
			  width: 100px;\n\
			  margin: 0 auto;\n\
			  /* border: 1px solid pink; */\n\
			}\n\
			.box {\n\
			  margin: 0 auto;\n\
			  display: flex;\n\
			  flex-direction: column;\n\
			  height: 700px;\n\
			  justify-content: center;\n\
			  justify-items: center;\n\
			  /* border: 1px solid green; */\n\
			}\n\
			.text {\n\
			  opacity: 0.5;\n\
			  line-height: 0%;\n\
			  align-self: center;\n\
			  /* border: 1px solid blue; */\n\
			  text-align: center;\n\
			  display: inline-block;\n\
			}\n\
			.line {\n\
			  height: 1px;\n\
			  background-color: black;\n\
			  margin: 1rem;\n\
			  opacity: 0.3;\n\
			}\n\
			/* .text_two {\n\
			  display: flex;\n\
			  text-align: center;\n\
			  display: inline-block;\n\
			  margin: 0 auto;\n\
			  font-size: 7px;\n\
			  line-height: 0%;\n\
			  font-family: \"Didact Gothic\", sans-serif;\n\
			} */\n\
		</style>\n\
		<meta charset=\"utf-8\" />\n\
	<title>";
	
	this->_error_page_html += GetCodeErrorConvert(error);
    
	this->_error_page_html += "</title>\n\
	<link href=\"https://profile.intra.42.fr/assets/42_logo_black-684989d43d629b3c0ff6fd7e1157ee04db9bb7a73fba8ec4e01543d650a1c607.png\" rel=\"icon\" type=\"image/png\">\n\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n\
	</head>\n\
	<body>\n\
		<div class=\"box\">\n\
		<div class=\"img\">\n\
			<img\n\
			src=\"https://oms-inventaires.com/wp-content/uploads/2020/09/Inventuren_Icons_Balken.svg\"\n\
			alt=\"logo\"\n\
			class=\"logo\"\n\
			/>\n\
		</div>\n\
		<div class=\"text\">\n\
			<h1>";
	this->_error_page_html += GetCodeErrorConvert(error);
	
	this->_error_page_html += "</h1>\n\
			</div>\n\
		</div>\n\
	</body>\n\
</html>";
    
    // std::sprintf(this->_code_char, "%d.html", this->_error_code);
    std::sprintf(this->_code_char, "%d.html", error);

    std::ofstream ofs;
    ofs.open(this->_code_char, std::ofstream::out);
    if(!ofs.is_open())
      return(EXIT_FAILURE);
    ofs << this->_error_page_html;
    ofs.close();
    
    return(EXIT_SUCCESS);
};

int 	Error::LoadDefaultErrorPage(int error, s_data &_d) {

	std::string _error = "./srcs/www" + _d._error_page.second;
	std::cout << "test " << std::endl;
	std::ifstream input(_error.c_str(), std::ios::binary);
	if (!input.is_open())
	{
		ErrorPage(error);
		std::ifstream input(this->_code_char, std::ios::binary);
		if (!input.is_open())
			return (error);
		input.seekg(0, std::ios::end);
		_d._content_size = input.tellg();
		input.seekg(0, std::ios::beg);
		std::sprintf(_d._c_size, "%lu", _d._content_size);
		if (_d._content_bin)
			free(_d._content_bin);
		if (!(_d._content_bin = (char*)malloc(sizeof(char) * (_d._content_size + 1))))
			return (EXIT_FAILURE);
		_d._content_bin[_d._content_size] = '\0';
		input.read(_d._content_bin, _d._content_size);
		return error;
	}
	input.seekg(0, std::ios::end);
	_d._content_size = input.tellg();
	input.seekg(0, std::ios::beg);
	std::sprintf(_d._c_size, "%lu", _d._content_size);
	if (_d._content_bin)
		free(_d._content_bin);
	if (!(_d._content_bin = (char*)malloc(sizeof(char) * (_d._content_size + 1))))
		return (EXIT_FAILURE);
	_d._content_bin[_d._content_size] = '\0';
	input.read(_d._content_bin, _d._content_size);
	return (error);
}


int		Error::MakeBinErrorPage(int error, s_data &_d) {


	if (_d._error_page.first == error) // .conf error page
	{
		LoadDefaultErrorPage(error, _d);
		return (error);
	}
	//si error page a ete set dans le .conf on utilise cette page pour lerreur 404
	if (ErrorPage(error) == EXIT_FAILURE)
		return (error);
	
	std::ifstream input(this->_code_char, std::ios::binary);
	if (!input.is_open())
		return (error);
	input.seekg(0, std::ios::end);
	_d._content_size = input.tellg();
	input.seekg(0, std::ios::beg);
	std::sprintf(_d._c_size, "%lu", _d._content_size);
		
	if (!(_d._content_bin = (char*)malloc(sizeof(char) * (_d._content_size + 1))))
		return (EXIT_FAILURE);
	_d._content_bin[_d._content_size] = '\0';
	input.read(_d._content_bin, _d._content_size);

	remove(this->_code_char);
	// if (_d._content_bin != NULL)
	// {
	// 	free(_d._content_bin);
	// 	_d._content_bin = NULL;
	// }
	
	//print
	// std::cout 	<< C_YELLOW << "--------------------Content_bin--------------------" << C_RESET << std::endl;
	// std::cout << C_YELLOW 	<< 		_d._content_bin << C_RESET << std::endl;
	// std::cout 	<< C_YELLOW << "--------------------Content_bin--------------------" << C_RESET << std::endl << std::endl;
	
	return (error);
};
