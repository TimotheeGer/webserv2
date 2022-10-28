// ************************************************************************** //
//                               	RESPONSE :                                //
// ************************************************************************** //

#include "../headers/Response.hpp"

// ************************************************************************** //
//                          CONSTRUCTOR DESTRUCTOR :                          //
// ************************************************************************** //


Response::Response(void) { return; };

Response::~Response(void) { return; };


// ************************************************************************** //
//                             MEMBER FONCTION: 	                          //
// ************************************************************************** //

/**
*	* 		Write Response from server and Print Response from server
*/

int Response::MakeResponse(Request &request_r, s_data &_d) {
	(void)request_r;
	std::cout << C_RED << "ERROR REPONSE 2.1" << C_RESET << std::endl;

	std::cout << C_BOLDYELLOW << "reponse size = [" << _d._response.size() << "]" << C_RESET
				<< " content size = [" << _d._content_size + 1 << "]" << C_RESET << std::endl;
	
	_d._response = WriteResponse(request_r, _d);
	
	if (!(_d._http = (char *)malloc(sizeof(char) * (_d._response.size() + (_d._content_size + 1)))))
	{
		std::cout << C_RED << "ERROR REPONSE EXIT" << C_RESET << std::endl;
		return (EXIT_FAILURE);
	}
	std::cout << C_RED << "ERROR REPONSE 2.2" << C_RESET << std::endl;



	memset(_d._http, 0, _d._response.size() + _d._content_size + 1);
	memcpy(_d._http, _d._response.c_str(), _d._response.size());
	memcpy(_d._http + strlen(_d._http), _d._content_bin, _d._content_size);
	// std::cout << C_RED << "_d._response = "  << _d._response << C_RESET << std::endl;
	std::cout << C_RED << "ERROR 2.3" << C_RESET << std::endl;
	
	if (_d._content_bin != NULL)
	{
		free(_d._content_bin);
		_d._content_bin = NULL;
	}
	// std::cout 	<< C_BOLDCYAN << "--------------------Response + Bin--------------------" << C_RESET << std::endl << std::endl;
	// std::cout	<< C_BOLDCYAN << 			_d._http << C_RESET << std::endl;
	// std::cout	<< C_BOLDCYAN << "--------------------Response + Bin--------------------" << C_RESET << std::endl << std::endl;
	return (EXIT_SUCCESS);
};

/**
*	* 		Create etiquette (header response)
*	TODO	Correct error and type handling 
*/

std::string Response::WriteResponse(Request &request_r, s_data &_d) {

	std::string http = "";
	std::string code = "";
	std::string content_type = "";
	std::string content_length = "";
	std::string etiquette = "";

	http = "HTTP/1.1 ";

	code = code + error_r.GetCodeErrorConvert(_d._error_code) + "\n";
	if (request_r.Get_Path().find(".php", 0) != std::string::npos)
	{
		content_length = content_length + "Content-Length: " + _d._c_size + "\n\n";
		etiquette = etiquette + http + code + FileIsPhp() + content_length;
	}
	else
	{
		if (_d._error_code == 200 || _d._error_code == 301) {
			std::cout << C_BOLDMAGENTA << "**********************TESTREPONSE1***********************" << C_RESET << std::endl;
			if (_d.autoindex == 1)
			{
				content_type = content_type + "Content-Type: " + "text/html" + "\n";
				// _d.autoindex = 0;
			}
			else
			{
				content_type = content_type + "Content-Type: " + this->mimes_r.getTypes(request_r.Get_Extension()) + "\n";			
			}
			content_length = content_length + "Content-Length: " + _d._c_size + "\n\n";

		}
		else if (error_r.IsErrorCode(_d._error_code) == EXIT_SUCCESS) {
			
			content_type = content_type + "Content-Type: "
										+ "text/html" + "\n";
			content_length = content_length + "Content-Length: " + _d._c_size + "\n\n";
		}
		etiquette = etiquette + http + code + content_type + content_length;
	}
	std::cout << C_BOLDMAGENTA << "WriteResponse ERROR 1" << C_RESET << std::endl;
	std::cout << C_BOLDMAGENTA << "Etiquette = [" << etiquette << "]" << C_RESET << std::endl;

	return (etiquette);
};


// ************************************************************************** //
//                             MEMBER FONCTION: utils                         //
// ************************************************************************** //
/**
*	*		Return size of the php file
*/

std::string	Response::FileIsPhp(void) {

	std::ifstream input("header_php", std::ios::binary);
	std::string _final_content;

	int size = 0;
	char c_size[11];
	char *content;
	if (!input.is_open())
		return NULL;

	input.seekg(0, std::ios::end);
	size = input.tellg();
	input.seekg(0, std::ios::beg);
	std::sprintf(c_size, "%u", size);
	if (!(content = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	content[size] = '\0';

	input.read(content, size);

	_final_content = content;
	if (content != NULL)
	{
		free(content);
		content = NULL;
	}
	return (_final_content);
};


// ************************************************************************** //
//                   MEMBER FONCTION: Print header response         		  //
// ************************************************************************** //

void			Response::PrintHeaderResponse(std::string etiquette) {

	std::cout << std::endl << C_BOLDMAGENTA << "-------------------------------header Response-----------------------" << C_RESET << std::endl;
	
	std::cout << C_BOLDMAGENTA << etiquette << C_RESET << std::endl;

	std::cout << C_BOLDMAGENTA << "-------------------------------header Response-----------------------" << C_RESET << std::endl << std::endl;

}