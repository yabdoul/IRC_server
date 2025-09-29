#include "Server.hpp"
#include <iostream>
#include "Reactor.hpp"
#include "IEventHandler.hpp"
#include "Client.hpp"
#include <algorithm>  
#include <memory>
#include "commandFactory.hpp"
/**
 * @brief Constructor for the Server class.
 * 
*/

const std::vector<Channel *  >&  Server::getChannelList() const
{
    return ChannelList ;
}

Server::Server() : IEventHandler(), _port(6667), _password("")
{     
	_ready2Send = false  ;   
	_serverName= SERVER_NAME ;   
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);  
	(listen_fd == -1) ? std::cout << "socket init problem" << std::endl : std::cout << "Socket inited Succefully\n";
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	int opt =1  ;  
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
		close(listen_fd);
		throw std::runtime_error("Failed to bind socket");
	}
	if (listen(listen_fd, 100) == -1) {
		close(listen_fd);
		throw std::runtime_error("Failed to listen on socket");
	}
	try
	{
		struct epoll_event ev;
		ev.events = EPOLLIN; 
		ev.data.fd = listen_fd;
		Reactor::getInstance().registre(ev, this);  
	}
	catch (std::exception &e)
	{
		close(listen_fd);
		std::cerr << e.what() << std::endl;
		throw;
	}
}  

void Server::initServer(int port, const std::string& password) {
	Server& server = getInstance();
	server._port = port;
	server._password = password;
}

const std::string& Server::getPassword() const {
	return _password;
}  
void Server::delUser(Client &cl  ) 
{ 
	std::cout<<"deleating User"<<cl.getClientFd()<<std::endl ;   
	for(std::vector<Client *>::iterator  it =  _clientList.begin() ;  it != _clientList.end()   ;  it++ )  
	{    
		std::cout<<"compare"<<cl.getClientFd()<<" & "<<(**it).getClientFd() <<std::endl ;   
		if(cl.getClientFd() ==  (**it).getClientFd() )   
			 _clientList.erase(it)  ;  
 	}  
	throw std::runtime_error("[DELUSER]: user Not Found") ;   
}
void Server::saveUser(Client *C )  
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
        ev.events = EPOLLIN ;
        ev.data.fd = client_fd;
        Reactor::getInstance().registre(ev, client);  
        // Store the pointer instead of copying the object
        // Remove this line that causes the copy:
        // this->_clientList.push_back(*client);   
    } catch (std::exception &e) {
        delete client;  // Clean up on error
        throw e;
    }
}   

bool Server::isNickAvai(std::string  nick   )  
{ 
		for(std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); it++)  
	{ 
		  if((*it)->userData()["nickname"] == nick)  
			  return false ;  
	}  ;       
	return true  ;   
} 
Client&  Server::getUser(std::string nickname )   
{  
	for(std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); it++)  
	{ 
		  if((*it)->userData()["nickname"] == nickname)  
			 return *(*it);   
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
Channel   Server::AddChannel(std::string  &ChName ,  Client  &owner    ) 
{    
	Channel ch(ChName , owner  ) ;   
	ChannelList.push_back(&ch) ;    
	return ch;   
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
Channel *    Server::IsChannelExist(std::string &ChName   )  
{  
	  for(std::vector<Channel *>::iterator it  =  ChannelList.begin() ;  it !=  ChannelList.end() ;  it++ )  
	  { 
		    if(ChName  ==  (*it)->getName() )     
			 return  *it  ;    
	  }   
	  return NULL ;    
} 

/**
 * @brief Unsubscribes the server from a specified channel by removing it from the channel list.
 * 
 * @param CName A reference to the name of the channel to unsubscribe from.
 * 
 * This function checks if the specified channel exists in the server's channel list.
 * If the channel exists, it removes the channel from the list.
 */
void  Server::UnsubscribeChannel(std::string &ChName )   
{  
	  for(std::vector<Channel *>::iterator it =  ChannelList.begin()  ;  it != ChannelList.end() ;  it++  )  
	  {    
		  if(ChName == (*it)->getName())  
		   	ChannelList.erase(it) ; 
	  }    ;   
	  throw std::runtime_error("Channel doesn't Exist") ;   
}      


void Server::callCommand(std::string& cmd, std::map<std::string, std::string>& params, Client& sender) 
{  
    Command* tmp = commandFactory::makeCommand(cmd);
    if (!tmp) {
        return;
    }
    
    if (dynamic_cast<ChannelCommand*>(tmp)) {
        // Make a local copy of the channel name
        std::string channelName(params["channel"]);
        
        // Get channel pointer and execute command while it's valid
        Channel* ch = IsChannelExist(channelName);
        if (ch) {
            ch->ExecuteCommand(*tmp, sender, params);
        }
    } else {
        sender.userCommand(*tmp, params);
    }
    
    // Clean up command object
    delete tmp;
	
}  ;      

void  Server::Respond2User(int Client_fd , std::string resp  )  
{        
		size_t len =  0 ;   
	    if(send(Client_fd ,resp.c_str() ,len ,   0 ) == -1)
	    { 
			throw std::runtime_error("[Response] :  Problem in send mechanisme") ;   
		}  
		/* 
		  
			[todo]  Implement  send logic here
		*/
} ;