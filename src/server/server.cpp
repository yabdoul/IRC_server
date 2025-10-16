#include "Server.hpp"
#include <iostream>
#include "Reactor.hpp"
#include "IEventHandler.hpp"
#include "Client.hpp"
#include <algorithm>  
#include <memory>
#include "commandFactory.hpp"
#include <fcntl.h>
#include <new>
/**
 * @brief Constructor for the Server class.
 * 
*/

const std::vector<Channel *  >&  Server::getChannelList() const
{
    return ChannelList ;
}

Server::Server() : IEventHandler(), listen_fd(-1), _serverName(SERVER_NAME), _port(0), _password("")
{     
	_ready2Send = false  ;   
	// Socket initialization will be done in initServer() with proper parameters
}  

void Server::initServer(int port, const std::string& password) {
	Server& server = getInstance();
	server._port = port;
	server._password = password;
	
	server.listen_fd = socket(AF_INET, SOCK_STREAM, 0);  
	if (server.listen_fd == -1) {
		throw std::runtime_error("Socket initialization failed");
	}
	
	if (fcntl(server.listen_fd, F_SETFL, O_NONBLOCK) == -1) {
		close(server.listen_fd);
		throw std::runtime_error("Failed to set server socket to non-blocking mode");
	}
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	int opt = 1;  
	setsockopt(server.listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(server.listen_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
		close(server.listen_fd);
		throw std::runtime_error("Failed to bind socket");
	}
	if (listen(server.listen_fd, 100) == -1) {
		close(server.listen_fd);
		throw std::runtime_error("Failed to listen on socket");
	}
	
	// Register server with reactor
	try {
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = server.listen_fd;
		Reactor::getInstance().registre(ev, &server);
	} catch (std::exception &e) {
		close(server.listen_fd);
		throw std::runtime_error("Failed to register server with reactor");
	}
}

const std::string& Server::getPassword() const {
	return _password;
}

void Server::removeClient(Client* client) {
    if (!client) return;
    
    // Remove client from all channels before deletion
    for(std::vector<Channel *>::iterator chIt = ChannelList.begin(); chIt != ChannelList.end(); ++chIt) {
        if(*chIt) {
            try {
                (*chIt)->removeUser(*client);
            } catch (...) {
                // Ignore errors if client not in this channel
            }
        }
    }
    
    // Remove from client list
    for(std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it) {
        if(*it == client) {
            _clientList.erase(it);
            break;
        }
    }
    
    // Get client fd before unregistering
    int client_fd = client->getClientFd();
    
    // Unregister from reactor
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = client_fd;
    try {
        Reactor::getInstance().unregistre(ev);
    } catch (...) {
        // Ignore errors during cleanup
    }
    
    // Delete the client object
    delete client;
}

void Server::cleanupDisconnectedClients() {
    std::vector<Client*> toDelete;
    
    try {
        // Find disconnected clients
        for(std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it) {
            if(*it && (*it)->isDisconnected()) {
                toDelete.push_back(*it);
            }
        }
        
        // Remove disconnected clients
        for(std::vector<Client*>::iterator it = toDelete.begin(); it != toDelete.end(); ++it) {
            if(*it) {
                removeClient(*it);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error during client cleanup: " << e.what() << std::endl;
        // Continue operation, don't rethrow
    }
}  
void Server::delUser(Client &cl  ) 
{ 
	for(std::vector<Client *>::iterator  it =  _clientList.begin() ;  it != _clientList.end()   ;  it++ )  
	{    
		if(cl.getClientFd() ==  (**it).getClientFd() )   
		{
			 _clientList.erase(it);  
			 return; // Successfully found and removed user
		}
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
	// Clean up all clients first (this will remove them from channels)
	for(std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it) {
		if(*it) {
			removeClient(*it);
		}
	}
	_clientList.clear();
	
	// Clean up all channels
	for(std::vector<Channel *>::iterator it = ChannelList.begin(); it != ChannelList.end(); ++it) {
		if(*it) {
			delete *it;
		}
	}
	ChannelList.clear();
	
	// Close listening socket
	if(listen_fd != -1) {
		close(listen_fd);
		listen_fd = -1;
	}
}

void Server::shutdown() {
	// Send QUIT messages to all clients before shutdown
	for(std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it) {
		if(*it) {
			try {
				(*it)->addMsg("ERROR :Server shutting down\r\n");
			} catch (...) {
				// Ignore errors during shutdown
			}
		}
	}
	
	// Clean up all resources (avoid iterator invalidation)
	while(!_clientList.empty()) {
		Client* client = _clientList.back();
		_clientList.pop_back();
		if(client) {
			// Remove from channels manually
			for(std::vector<Channel *>::iterator chIt = ChannelList.begin(); chIt != ChannelList.end(); ++chIt) {
				if(*chIt) {
					try {
						(*chIt)->removeUser(*client);
					} catch (...) {
						// Ignore errors during cleanup
					}
				}
			}
			
			// Get client fd before cleanup
			int client_fd = client->getClientFd();
			
			// Unregister from reactor
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = client_fd;
			try {
				Reactor::getInstance().unregistre(ev);
			} catch (...) {
				// Ignore errors during cleanup
			}
			
			// Delete client
			delete client;
		}
	}
	
	for(std::vector<Channel *>::iterator it = ChannelList.begin(); it != ChannelList.end(); ++it) {
		if(*it) {
			delete *it;
		}
	}
	ChannelList.clear();
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

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(listen_fd, (sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
        throw std::runtime_error("fatal : Accept() ");

    // Set client socket to non-blocking mode (MANDATORY requirement)
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
        close(client_fd);
        throw std::runtime_error("Failed to set client socket to non-blocking mode");
    }

    Client *client = NULL;
    try {
        client = new Client(client_fd);
        struct epoll_event ev;
        ev.events = EPOLLIN ;
        ev.data.fd = client_fd;
        Reactor::getInstance().registre(ev, client);  
        // Add client to the client list for tracking
        this->_clientList.push_back(client);   
    } catch (const std::bad_alloc &e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        if (client) delete client;
        close(client_fd);
        throw std::runtime_error("Failed to allocate memory for new client");
    } catch (const std::exception &e) {
        std::cerr << "Client registration failed: " << e.what() << std::endl;
        if (client) delete client;
        close(client_fd);
        throw;
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
		  if((*it)->getNickName() == nickname)  
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

Channel*   Server::AddChannel(std::string  &ChName ,  Client  &owner    ) 
{              
	  for(std::vector<Channel * >::iterator it  = ChannelList.begin() ;  it !=  ChannelList.end() ;  it++ )   
	  {  
		 if((**it).getName() == ChName )  
		 	return *it  ;  
	  } ;  
	  Channel * newChannel  =  new Channel(ChName , owner) ;    
	  ChannelList.push_back(newChannel)     ; 
	  return  newChannel ;      
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
    Command* Cmd = commandFactory::makeCommand(cmd) ;  
	if(Cmd) 
	{ 
        try {
            if(dynamic_cast<ChannelCommand  *> (Cmd) )   
            { 
                if (params.find("channel") != params.end()) {
                    Channel  *  target = sender.getChannel(params["channel"])     ;         
                    if(!target  &&     cmd  == "JOIN")  
                    { 
                        target = Server::getInstance().AddChannel(params["channel"] , sender) ;     
                        sender.subscribe2channel(*target) ;   
                    }         
                    if( target)    
                       target->ExecuteCommand(*Cmd , sender  , params) ;  
                }
            }    
            else {  
                sender.userCommand(*Cmd  , params  ) ;   
            }  
            Server::getInstance().beReady2Send() ;
        } catch (const std::exception& e) {
            delete Cmd;
            throw; // Re-throw the exception after cleanup
        }
        delete Cmd ;   
	}
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