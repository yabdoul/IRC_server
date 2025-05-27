#include "Server.hpp"
#include <iostream>
#include "Reactor.hpp"
#include "IEventHandler.hpp"
#include "Client.hpp"
Server::Server() : IEventHandler()
{
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	(listen_fd == -1) ? std::cout << "socket init problem" << std::endl : std::cout << "Socket inited Succefully\n";
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(4444); // port 8080
	bind(listen_fd, (struct sockaddr *)&address, sizeof(address));
	listen(listen_fd, 100);
	try
	{
		struct epoll_event ev;
		ev.events = EPOLLIN; // EPOLL_IN FOR INCOMING CONNECTIONS
		ev.data.fd = listen_fd;
		Reactor::getInstance().registre(ev, this);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

int Server::getListenFd()
{
	return listen_fd;
}
Server::~Server()
{
	// close listening Here ---- & destory socket
}

void Server::handle_event(epoll_event ev)
{
	(void)ev;

	std::cout << "Accept handle event called" << std::endl;
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(listen_fd, (sockaddr *)&client_addr, &client_len);
	if (client_fd == -1)
		throw std::runtime_error("fatal : Accept() ");

	Client *client = new Client(client_fd);
	try
	{
		struct epoll_event ev;
		ev.events = EPOLLIN | EPOLLOUT;
		ev.data.fd = client_fd;
		Reactor::getInstance().registre(ev, client);
	} catch (std::exception &e) {
		throw e;
	}
}
Server &Server::getInstance()
{
	static Server instance;
	return instance;
}
