#include "Server.hpp"
#include <iostream>
#include "Reactor.hpp"
#include "IEventHandler.hpp"
#include "Client.hpp"
#include <algorithm>  

/**
 * @brief Constructor for the Server class.
 * 
 * Initializes a server socket, binds it to a specified address and port, 
 * and sets it to listen for incoming connections. Additionally, registers 
 * the socket with the Reactor for event handling.
 * 
 * @details
 * - Creates a socket using the `socket` function.
 * - Binds the socket to `INADDR_ANY` and port 4444.
 * - Sets the socket to listen with a backlog of 100 connections.
 * - Registers the socket with the Reactor for EPOLLIN events.
 * - Handles exceptions during registration with the Reactor.
 * 
 * @throws std::exception If an error occurs during registration with the Reactor.
 */
Server::Server() : IEventHandler()
{   
	_serverName= SERVER_NAME ;   
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	(listen_fd == -1) ? std::cout << "socket init problem" << std::endl : std::cout << "Socket inited Succefully\n";
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(6667); // port 8080
	bind(listen_fd, (struct sockaddr *)&address, sizeof(address));
	listen(listen_fd, 100) ;
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

void Server::saveUser(Client &C )  
{ 
	if(std::find(_clientList.begin() , _clientList.end() ,  C )   ==  _clientList.end( ) )  
		_clientList.push_back(C) ;    
	else 
		throw std::runtime_error("Client Already Registred" ) ;   	
}

int Server::getListenFd()
{
	return listen_fd;
}
Server::~Server()
{
	// close listening Here ---- & destory socket
}

/**
 * @brief Handles an incoming event on the server's listening socket.
 * 
 * This function is responsible for accepting a new client connection,
 * creating a Client object for the connection, and registering the client
 * with the Reactor for event handling.
 * 
 * @param ev The epoll_event structure containing information about the event.
 *           Currently unused in this implementation.
 * 
 * @throws std::runtime_error If the accept() system call fails.
 * @throws std::exception If an error occurs during client registration with the Reactor.
 */
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
		saveUser(*client) ;     
	} catch (std::exception &e) {
		throw e;
	}
}  

const  Client& Server::getUser(std::string nickname ) const  
{  
	for(std::vector<Client>::const_iterator it =  _clientList.begin() ;  it  !=  _clientList.end() ;  it++)  
	{ 
		  if((it)->userData()["nick"]  ==   nickname  )  
		  	 return  (*it)    ;   
	}  ;      
	throw std::runtime_error("User Not found") ;   
} 
Server &Server::getInstance()
{
	static Server instance;
	return instance;
}
  
/**
 * @brief Adds a new channel to the server's channel list.
 * 
 * This function creates a new channel and associates it with the given
 * channel name. The channel is then added to the server's ChannelList.
 * 
 * @param CnName Reference to the name of the channel to be added.
 *               This name will be used as the key in the ChannelList map.
 */
void Server::AddChannel(std::string  &CnName  ) 
{    
	Channel ch ;   
	this->ChannelList.insert(std::make_pair(CnName, ch));
}  

/**
 * @brief Checks if a channel exists in the server's channel list.
 * 
 * This function searches for a channel by its name in the server's channel list.
 * If the channel exists, it returns a pointer to the channel object. If the channel
 * does not exist, it throws a ServerException and logs the error message to the standard error stream.
 * 
 * @param ChName The name of the channel to search for.
 * @return Channel* Pointer to the channel object if found, otherwise NULL.
 * 
 * @throws ServerException If the channel is not found in the channel list.
 */
Channel *  Server::IsChannelExist(std::string &ChName   )  
{  
	std::map <std::string , Channel>::iterator it =   this->ChannelList.find(ChName);   
	try {  
		 if( it  !=  ChannelList.end() )  
				return &it->second ;   
		else 
			throw ServerException("Channel Not Found") ;   
	} 
	catch(ServerException &e )
	{ 
			std::cerr<<e.what()  ;    
			return NULL ;   
	}
} 

/**
 * @brief Unsubscribes the server from a specified channel by removing it from the channel list.
 * 
 * @param CName A reference to the name of the channel to unsubscribe from.
 * 
 * This function checks if the specified channel exists in the server's channel list.
 * If the channel exists, it removes the channel from the list.
 */
void  Server::UnsubscribeChannel(std::string &CName)   
{  
	    if(IsChannelExist(CName))
		{  
			 this->ChannelList.erase(CName) ;  
		} 
}  