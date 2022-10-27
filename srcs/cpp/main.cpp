#include "../headers/Server.hpp"

int main(int ac, char **av) {

	Conf 		conf_r;
	if (conf_r.WorkConf(ac, av) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	Server		server_r;
	server_r.WaitConnection(conf_r.GetMapServe());

    return 0;
}
