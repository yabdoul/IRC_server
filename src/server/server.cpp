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


const std::vector<Channel *  >&  Server::getChannelList() const
{
    return ChannelList ;
}

Server::Server() : IEventHandler(), listen_fd(-1), _serverName(SERVER_NAME), _port(0), _password("")
{     
	_ready2Send = false  ;   
	
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
    
    
    for(std::vector<Channel *>::iterator chIt = ChannelList.begin(); chIt != ChannelList.end(); ++chIt) {
        if(*chIt) {
            try {
                (*chIt)->removeUser(*client);
            } catch (...) {
                
            }
        }
    }
    
    
    for(std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it) {
        if(*it == client) {
            _clientList.erase(it);
            break;
        }
    }
    
    
    int client_fd = client->getClientFd();
    
    
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = client_fd;
    try {
        Reactor::getInstance().unregistre(ev);
    } catch (...) {
        
    }
    
    
    delete client;
}

void Server::cleanupDisconnectedClients() {
    std::vector<Client*> toDelete;
    
    try {
        
        for(std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it) {
            if(*it && (*it)->isDisconnected()) {
                toDelete.push_back(*it);
            }
        }
        
        
        for(std::vector<Client*>::iterator it = toDelete.begin(); it != toDelete.end(); ++it) {
            if(*it) {
                removeClient(*it);
            }
        }
    } catch (const std::exception &e) {
        
    }
}  
void Server::delUser(Client &cl  ) 
{ 
	for(std::vector<Client *>::iterator  it =  _clientList.begin() ;  it != _clientList.end()   ;  it++ )  
	{    
		if(cl.getClientFd() ==  (**it).getClientFd() )   
		{
			 _clientList.erase(it);  
			 return; 
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
	
	while (!_clientList.empty()) {
		Client* client = _clientList.back();
		_clientList.pop_back();
		if (client) {
			
			for (std::vector<Channel *>::iterator chIt = ChannelList.begin(); chIt != ChannelList.end(); ++chIt) {
				if (*chIt) {
					try {
						(*chIt)->removeUser(*client);
					} catch (...) {
						
					}
				}
			}

			
			int client_fd = client->getClientFd();
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = client_fd;
			try {
				Reactor::getInstance().unregistre(ev);
			} catch (...) {
				
			}

			
			delete client;
		}
	}

	
	while (!ChannelList.empty()) {
		Channel* ch = ChannelList.back();
		ChannelList.pop_back();
		if (ch) delete ch;
	}

	
	if (listen_fd != -1) {
		close(listen_fd);
		listen_fd = -1;
	}
}

void Server::shutdown() {
	
	for(std::vector<Client *>::iterator it = _clientList.begin(); it != _clientList.end(); ++it) {
		if(*it) {
			try {
				(*it)->addMsg("ERROR :Server shutting down\r\n");
			} catch (...) {
				
			}
		}
	}
	
	
	while(!_clientList.empty()) {
		Client* client = _clientList.back();
		_clientList.pop_back();
		if(client) {
			
			for(std::vector<Channel *>::iterator chIt = ChannelList.begin(); chIt != ChannelList.end(); ++chIt) {
				if(*chIt) {
					try {
						(*chIt)->removeUser(*client);
					} catch (...) {
						
					}
				}
			}
			
			
			int client_fd = client->getClientFd();
			
			
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = client_fd;
			try {
				Reactor::getInstance().unregistre(ev);
			} catch (...) {
				
			}
			
			
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


void Server::handle_event(epoll_event ev)
{
    (void)ev;

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(listen_fd, (sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
        throw std::runtime_error("fatal : Accept() ");

    
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
        
        this->_clientList.push_back(client);   
    } catch (const std::bad_alloc &e) {
        if (client) delete client;
        close(client_fd);
        throw std::runtime_error("Failed to allocate memory for new client");
    } catch (const std::exception &e) {
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


Channel *    Server::IsChannelExist(std::string &ChName   )  
{  
	  for(std::vector<Channel *>::iterator it  =  ChannelList.begin() ;  it !=  ChannelList.end() ;  it++ )  
	  { 
		    if(ChName  ==  (*it)->getName() )     
			 return  *it  ;    
	  }   
	  return NULL ;    
} 



void  Server::UnsubscribeChannel(std::string &ChName )   
{  
	for (std::vector<Channel *>::iterator it = ChannelList.begin(); it != ChannelList.end(); ++it) {
		if (ChName == (*it)->getName()) {
			Channel* ch = *it;
			
			delete ch;
			ChannelList.erase(it);
			return;
		}
	}
	throw std::runtime_error("Channel doesn't Exist");
}      


void Server::callCommand(std::string& cmd, std::map<std::string, std::string>& params, Client& sender) 
{    
    Command* Cmd = commandFactory::makeCommand(cmd) ;  
	if(Cmd) 
	{ 
        try {
            if(dynamic_cast<ChannelCommand  *> (Cmd) )   
            {   
				if(params.count("count") > 0 ) 
				{  
				for(int i  =  0  ;  i  <   atoi(params["count"].c_str()) ;  i++ )   
					{   
						params["channel"]  =  params["channel_"+i]  ;     
						params["key"]  = params["key_"+i] ;   
						if (params.find("channel") != params.end()) {
							Channel  *  target = sender.getChannel(params["channel"])      ;     
							if(!target  &&     cmd  == "JOIN")  
							{   
								target = Server::getInstance().AddChannel(params["channel"] , sender) ;     
								sender.subscribe2channel(*target) ;   
							}         
							if( target)    
							{      
								target->ExecuteCommand(*Cmd , sender  , params) ;      
							} 
						}  
					}
				}  
				else{
				if (params.find("channel") != params.end()) {
							Channel  *  target = sender.getChannel(params["channel"])      ;     
							if(!target  &&     cmd  == "JOIN")  
							{   
								target = Server::getInstance().AddChannel(params["channel"] , sender) ;     
								sender.subscribe2channel(*target) ;   
							}         
							if( target)    
							{      
								target->ExecuteCommand(*Cmd , sender  , params) ;      
							} 
						}  
				}
				
			}
            else {  
                sender.userCommand(*Cmd  , params  ) ;   
            }  
            Server::getInstance().beReady2Send() ;
        } catch (const std::exception& e) {
            delete Cmd;
            throw; 
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
		
} ;