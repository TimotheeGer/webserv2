#ifndef REQUEST_HPP
#define REQUEST_HPP

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
#include "Response.hpp"
#include "Colors.hpp"
#include "Global.hpp"

class Request {

	public:
		
		Request();
		~Request();

		int										GetElementRequest(s_data &d, std::map<int, std::map<std::string, t_scop> > &map_server);
		bool									RequestIsFull(s_data &d);
		int										GetBoundary(s_data &d);
		int 									DeleteFirstLine(void);
		int 									HandleMethod(s_data &d);
		
		int 									TreatmentPath(s_data &d);
		int										ClearPath(s_data &d);
		int										GetFirstPath(s_data &d);


		//get
		std::string								Get_AllRequest(void);
		std::map<std::string, std::string>		Get_MapRequest(void);
		std::string								Get_Method(void);
		std::string								Get_Path(void);
		std::string								Get_Host(void);
		std::string								Get_ContentLength(void);
		std::string								Get_Accept(void);
		std::string								Get_AccepEncoding(void);
		std::string								Get_AcceptLanguage(void);
		std::string								Get_Connection(void);
		std::string								Get_SecFetchDest(void);
		std::string								Get_SecFetchMode(void);
		std::string								Get_SecFetchSite(void);
		std::string								Get_SecFetchUser(void);
		std::string								Get_SecGpc(void);
		std::string								Get_UpgradeInsecureRequests(void);
		std::string								Get_UserAgent(void);
		std::string								Get_ContentType(void);
		std::string								Get_Extension(void);
		std::string								Get_QueryString(void);
		std::string								Get_CacheControl(void);
		std::string								Get_FirstPath(void);
		std::string								Get_OnlyBody(void);
		//set
		void									Set_Path(std::string set);

		void 									ClearRequest(void);
		
		std::map<std::string, std::string>		_MapRequest;

		std::string								string_post;

		std::string								_RequestBody;
		std::string								_AllRequest;
		std::string								_HeaderBody;
		std::string								_OnlyBody;
		std::string								_FileNameBody;
		std::string								_FirstPath;


	private:
	
		std::vector<std::string> 				_TabRequest;
		std::vector<std::string> 				TabBody;

		// std::string 							GetMethod(std::map<std::string, std::string> &map_req);

		int										GetBodyInRequest(s_data &d);
		int										GetAllElement(void);
		int										TreatmentFirstLigne(void);
		int										TreatmentAfterFirstLigne(void);
		int										GetHeaderBody(s_data &d);
		int										GetOnlyBody(s_data &d);
		int										CheckBodySizeConf(s_data &d, std::map<std::string, t_scop> &MapConf);
		int										CheckUpload(s_data &d, std::map<std::string, t_scop> &MapConf);
		int										UploadFile(s_data &d);
		int 									GetCorrectMethod();
		std::map<std::string, t_scop> & 		GetMapConf(s_data &d, std::map<int, std::map<std::string, t_scop> > &map_server);
		std::string								GetHostForConf(std::string host);

		//utils
		int										Split_tab(std::string Request, std::vector<std::string> &tab, std::string sep);
		int										is_sep(std::string sep, char c);
		int										ParseBodyRequest(void);

		//print
		void									PrintBodyRequest(s_data &d);
		void									PrintTab(std::vector<std::string> &tab, std::string str);
		void									PrintMapRequest(void);
		void									PrintHeader(std::string header);
		void									PrintRequestFull(std::string request);
		void									PrintHeaderBody(void);
		void									PrintOnlyBody(void);
		void									PrintTabHeaderBody(std::vector<std::string> &tab);
		int										ChecklimitExcept(s_data &_d, std::map<std::string, t_scop> &MapConf);
		int										CheckPathUpload(std::string path);

};

#endif