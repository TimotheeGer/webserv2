#include "../headers/Server.hpp"

int main(int ac, char **av) {

	std::cout << std::endl << C_BOLDCYAN << "Launch Webserv \U0001F680" << C_RESET << std::endl << std::endl;

	Conf 		conf_r;
	if (conf_r.WorkConf(ac, av) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	Server		server_r;
	if (server_r.WaitConnection(conf_r.GetMapServe()) == EXIT_FAILURE)
		return (EXIT_FAILURE);

    return 0;
}
