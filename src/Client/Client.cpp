#include "Client.hpp"
#include <fcntl.h>
#include <unistd.h>  
#include "Parser.hpp"  
#include <cstdlib>  
#include <sstream> 
#include "Channel.hpp"
#include "Server.hpp"
#include <errno.h>
#include <cstring>

inline std::string SSTR(int x) {
    std::ostringstream oss;
    oss << std::dec << x;
    return oss.str();
}



class ClientException : public std::exception
{
private:
    const std::string _reason;

public:
    ClientException(const std::string &reason) : _reason("[ClientException]: " + reason)
    {
    }
    ~ClientException() throw() {}
    virtual char const *what() const throw()
    {
        return _reason.c_str();
    }
};  


Client::Client(int client_fd, const std::string& Nick, const std::string& User, const std::string& Pass)
    {    
         _client_fd = client_fd ;  
         _Nick =  Nick ;  
         _User = User  ;  
         _Pass = Pass ;
         _state = CONNECTING;
    }

Client::Client(int client_fd) :_client_fd(client_fd), _state(CONNECTING)
{}

Client::~Client()
{   if(_client_fd) close(_client_fd);
}

Client::Client(const Client &other):IEventHandler(other)
{  
     _Nick =  other._Nick ;   
     _Pass = other._Pass ;     
}  

void Client::rcvMsg(std::string &Msg) const  
{ 

    if (_client_fd == -1) {
        return;
    }
    
    ssize_t bytes_sent = send(_client_fd, Msg.c_str(), Msg.size(), MSG_DONTWAIT);
    
    if (bytes_sent < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            std::cout << "Send buffer full for client " << _Nick << std::endl;
        } else {
            std::cout << "Send error for client " << _Nick << ": " << strerror(errno) << std::endl;
        }
    } else if (bytes_sent < static_cast<ssize_t>(Msg.size())) {
        std::cout << "Partial send for client " << _Nick << ": " << bytes_sent << "/" << Msg.size() << " bytes" << std::endl;
    }
}  ;   

  std::map<std::string ,  std::string> Client::userData () const   
{
        std::map<std::string ,  std::string> result ;   
        result["_client_fd"] = SSTR(_client_fd).c_str()      ;    
        result["nickname"]   = _Nick ;    
        result["user"] =  _User ;    
        result["server"] = "ascasc" ;   
        return  result  ;   
}  ;     
 

void Client::subscribe2channel(Channel &ch )  
{   
    if(std::find(_subscribed2Channel.begin() ,  _subscribed2Channel.end()  ,  &ch) == _subscribed2Channel.end() )
      _subscribed2Channel.push_back(&ch) ;    
    else 
        throw std::runtime_error("[Already in The Channel]") ;   
} ;   

void  Client::userCommand(Command  & cmd  , std::map<std::string ,  std::string >&params   )  
{ 
    try{  
         cmd.execute(*this ,   params );  
    }  
    catch(std::exception &e  ) 
    { 
         std::cerr<<e.what()<<std::endl ;    
    }
}  ;   
 
void Client::handle_event(epoll_event e)
{
    if (e.events & EPOLLIN) {
        std::vector<char> buffer(1024, '\0');  
        ssize_t n = recv(_client_fd, (void *)buffer.data(), buffer.size(), 0);
        
        if (n > 0) {  
            std::cout << "RAW string: " << std::string(buffer.data(), n) << std::endl;
            
            _messageBuffer.append(buffer.data(), n);
            
            size_t pos;
            while ((pos = _messageBuffer.find("\r\n")) != std::string::npos) {
                std::string command = _messageBuffer.substr(0, pos);
                _messageBuffer.erase(0, pos + 2);
                
                if (!command.empty()) {
                    processCommand(command);
                }
            }
        } else if (n == 0) {
            std::cout << "Client " << _Nick << " disconnected" << std::endl;
            close(_client_fd);
            _client_fd = -1;
            return;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return;
            } else {
                std::cerr << "recv() error for client " << _Nick << ": " << strerror(errno) << std::endl;
                close(_client_fd);
                _client_fd = -1;
                return;
            }
        }
    
    } else if (e.events & EPOLLOUT) {
        // TODO: Implement send buffer handling for better performance
        // For now, basic implementation
    }
}

// Authentication
bool Client::isRegistered() const {
    return _state == REGISTERED;
}

Client::ClientState Client::getState() const {
    return _state;
}

void Client::setState(ClientState state) {
    _state = state;
}

const std::string& Client::getNickname() const {
    return _Nick;
}

const std::string& Client::getUsername() const {
    return _User;
}

const std::string& Client::getRealName() const {
    return _realName;
}

int Client::getClientFd() const {
    return _client_fd;
}

void Client::processCommand(const std::string& command) {
    Parser& parser = Parser::getInstance();
    parser.parse(command);
    
    std::string cmd = parser.getCommand();
    const std::map<std::string, std::string>& params = parser.getParams();
    
    try {
        if (cmd == "PASS") {
            std::map<std::string, std::string>::const_iterator it = params.find("password");
            if (it != params.end()) {
                handlePassCommand(it->second);
            }
        } else if (cmd == "NICK") {
            std::map<std::string, std::string>::const_iterator it = params.find("nickname");
            if (it != params.end()) {
                handleNickCommand(it->second);
            }
        } else if (cmd == "USER") {
            std::map<std::string, std::string>::const_iterator it_user = params.find("username");
            std::map<std::string, std::string>::const_iterator it_real = params.find("realname");
            std::map<std::string, std::string>::const_iterator it_host = params.find("hostname");
            std::map<std::string, std::string>::const_iterator it_serv = params.find("servername");
            
            if (it_user != params.end() && it_real != params.end()) {
                std::string hostname = (it_host != params.end()) ? it_host->second : "localhost";
                std::string servername = (it_serv != params.end()) ? it_serv->second : "localhost";
                handleUserCommand(it_user->second, hostname, servername, it_real->second);
            }
        } else if (!isRegistered()) {
            std::string error = ":server 451 * :You have not registered\r\n";
            rcvMsg(error);
        } else {
            if (cmd == "PRIVMSG") {
                handlePrivmsgCommand(params);
            } else if (cmd == "JOIN") {
                handleJoinCommand(params);
            } else if (cmd == "KICK") {
                handleKickCommand(params);
            } else if (cmd == "INVITE") {
                handleInviteCommand(params);
            } else if (cmd == "TOPIC") {
                handleTopicCommand(params);
            } else if (cmd == "MODE") {
                handleModeCommand(params);
            } else if (cmd == "PART") {
                handlePartCommand(params);
            } else {
                std::string error = ":server 421 " + _Nick + " " + cmd + " :Unknown command\r\n";
                rcvMsg(error);
            }
        }
        
    } catch (std::exception& e) {
        std::cerr << "Error processing command: " << e.what() << std::endl;
    }
}

void Client::handlePassCommand(const std::string& password) {
    if (_state != CONNECTING) {
        std::string error = ":server 462 " + _Nick + " :You may not reregister\r\n";
        rcvMsg(error);
        return;
    }
    
    if (password != Server::getInstance().getPassword()) {
        std::string error = ":server 464 * :Password incorrect\r\n";
        rcvMsg(error);
        return;
    }
    
    _Pass = password;
    _state = PASSWORD_SET;
}

void Client::handleNickCommand(const std::string& nickname) {
    if (_state == CONNECTING) {
        std::string error = ":server 451 * :You have not registered\r\n";
        rcvMsg(error);
        return;
    }
    
    // Nickname duplicate
    try {
        Server::getInstance().getUser(nickname);
        std::string error = ":server 433 * " + nickname + " :Nickname is already in use\r\n";
        rcvMsg(error);
        return;
    } catch (...) {
        // Not found, good
    }
    
    std::string oldNick = _Nick;
    _Nick = nickname;
    
    if (_state == PASSWORD_SET) {
        _state = NICK_SET;
    } else if (_state == REGISTERED) {
        std::string msg = ":" + oldNick + "!" + _User + "@" + _hostname + 
                         " NICK " + nickname + "\r\n";
        
        // Send to all channels user is in
        for (std::vector<Channel*>::iterator it = _subscribed2Channel.begin();
             it != _subscribed2Channel.end(); ++it) {
            (*it)->broadcastMessage(msg);
        }
        
        rcvMsg(msg);
    }
}

void Client::handleUserCommand(const std::string& username, const std::string& hostname,
                              const std::string& servername, const std::string& realname) {
    if (_state != NICK_SET) {
        std::string error = ":server 451 * :You have not registered\r\n";
        rcvMsg(error);
        return;
    }
    
    (void)servername;
    
    _User = username;
    _hostname = hostname;
    _realName = realname;
    _state = REGISTERED;
    
    sendWelcomeSequence();
}

void Client::sendWelcomeSequence() {
    if (_state != REGISTERED) return;
    
    std::string servername = "irc_server_ysf";
    
    std::string welcome = ":" + servername + " 001 " + _Nick + " :Welcome to the IRC network, " + _Nick + "\r\n";
    rcvMsg(welcome);
    
    std::string yourhost = ":" + servername + " 002 " + _Nick + " :Your host is " + servername + ", running version 1.0\r\n";
    rcvMsg(yourhost);
    
    std::string created = ":" + servername + " 003 " + _Nick + " :This server was created today\r\n";
    rcvMsg(created);
    
    std::string myinfo = ":" + servername + " 004 " + _Nick + " " + servername + " 1.0 o o\r\n";
    rcvMsg(myinfo);
}

void Client::handlePrivmsgCommand(const std::map<std::string, std::string>& params) {
    std::map<std::string, std::string>::const_iterator target_it = params.find("target_type");
    std::map<std::string, std::string>::const_iterator msg_it = params.find("message");
    
    if (msg_it == params.end()) {
        std::string error = ":server 412 " + _Nick + " :No text to send\r\n";
        rcvMsg(error);
        return;
    }
    
    if (target_it != params.end() && target_it->second == "channel") {
        // For channel
        std::map<std::string, std::string>::const_iterator ch_it = params.find("channel");
        if (ch_it != params.end()) {
            std::string channelName = ch_it->second;
            Channel* channel = Server::getInstance().IsChannelExist(channelName);
            if (channel) {
                if (channel->isUserInChannel(*this)) {
                    std::string message = ":" + _Nick + "!" + _User + "@" + _hostname + 
                                        " PRIVMSG #" + channelName + " :" + msg_it->second + "\r\n";
                    channel->broadcastMessage(message, this);
                    std::cout << "Broadcasting to channel #" << channelName << ": " << msg_it->second << std::endl;
                } else {
                    std::string error = ":server 404 " + _Nick + " #" + channelName + " :Cannot send to channel\r\n";
                    rcvMsg(error);
                }
            } else {
                std::string error = ":server 403 " + _Nick + " #" + channelName + " :No such channel\r\n";
                rcvMsg(error);
            }
        }
    } else {
        // For user
        std::map<std::string, std::string>::const_iterator nick_it = params.find("nickname");
        if (nick_it != params.end()) {
            try {
                Client& targetClient = Server::getInstance().getUser(nick_it->second);
                std::string message = ":" + _Nick + "!" + _User + "@" + _hostname + 
                                    " PRIVMSG " + nick_it->second + " :" + msg_it->second + "\r\n";
                targetClient.rcvMsg(message);
                std::cout << "Private message from " << _Nick << " to " << nick_it->second << ": " << msg_it->second << std::endl;
            } catch (...) {
                std::string error = ":server 401 " + _Nick + " " + nick_it->second + " :No such nick\r\n";
                rcvMsg(error);
            }
        }
    }
}

void Client::handleJoinCommand(const std::map<std::string, std::string>& params) {
    std::map<std::string, std::string>::const_iterator ch_it = params.find("channel");
    if (ch_it == params.end()) {
        std::string error = ":server 461 " + _Nick + " JOIN :Not enough parameters\r\n";
        rcvMsg(error);
        return;
    }
    
    std::string channelName = ch_it->second;
    Channel* channel = Server::getInstance().IsChannelExist(channelName);
    
    if (!channel) {
        Server::getInstance().AddChannel(channelName);
        channel = Server::getInstance().IsChannelExist(channelName);
        std::cout << "Created new channel #" << channelName << " with " << _Nick << " as operator" << std::endl;
    }
    
    try {
        channel->enterChannel(*this);
        subscribe2channel(*channel);
        
        std::string joinMsg = ":" + _Nick + "!" + _User + "@" + _hostname + " JOIN #" + channelName + "\r\n";
        rcvMsg(joinMsg);
        
        // RPL_NAMREPLY (353)
        std::string names = ":server 353 " + _Nick + " = #" + channelName + " :" + _Nick + "\r\n";
        rcvMsg(names);
        
        // RPL_ENDOFNAMES (366)
        std::string endNames = ":server 366 " + _Nick + " #" + channelName + " :End of /NAMES list\r\n";
        rcvMsg(endNames);
        
        std::cout << _Nick << " joined channel #" << channelName << std::endl;
        
    } catch (std::exception& e) {
        std::string error = ":server 403 " + _Nick + " #" + channelName + " :" + e.what() + "\r\n";
        rcvMsg(error);
    }
}

void Client::handleKickCommand(const std::map<std::string, std::string>& params) {
    std::map<std::string, std::string>::const_iterator ch_it = params.find("channel");
    std::map<std::string, std::string>::const_iterator nick_it = params.find("nickname");
    
    if (ch_it == params.end() || nick_it == params.end()) {
        std::string error = ":server 461 " + _Nick + " KICK :Not enough parameters\r\n";
        rcvMsg(error);
        return;
    }
    
    std::string channelName = ch_it->second;
    // Normalize # name
    if (!channelName.empty() && channelName[0] != '#') {
        channelName = "#" + channelName;
    }
    
    Channel* channel = Server::getInstance().IsChannelExist(channelName);
    
    if (!channel) {
        std::string error = ":server 403 " + _Nick + " " + channelName + " :No such channel\r\n";
        rcvMsg(error);
        return;
    }
    
    // kicker is in channel
    if (!channel->isUserInChannel(*this)) {
        std::string error = ":server 442 " + _Nick + " " + channelName + " :You're not on that channel\r\n";
        rcvMsg(error);
        return;
    }
    
    // kicker is operator
    if (!channel->isOp(*this)) {
        std::string error = ":server 482 " + _Nick + " " + channelName + " :You're not channel operator\r\n";
        rcvMsg(error);
        return;
    }
    
    // target user exists
    Client* targetClient = NULL;
    try {
        targetClient = &Server::getInstance().getUser(nick_it->second);
    } catch (...) {
        std::string error = ":server 401 " + _Nick + " " + nick_it->second + " :No such nick\r\n";
        rcvMsg(error);
        return;
    }
    
    // target is on the channel
    if (!channel->isUserInChannel(*targetClient)) {
        std::string error = ":server 441 " + _Nick + " " + nick_it->second + " " + channelName + " :They aren't on that channel\r\n";
        rcvMsg(error);
        return;
    }
    
    // Get kick reason
    std::string reason = "No reason given";
    std::map<std::string, std::string>::const_iterator reason_it = params.find("reason");
    if (reason_it != params.end()) {
        reason = reason_it->second;
    }
    
    // (No exceptions left)
    channel->kickUser(*this, *targetClient, reason);
    std::cout << _Nick << " kicked " << nick_it->second << " from " << channelName << std::endl;
}

void Client::handleInviteCommand(const std::map<std::string, std::string>& params) {
    std::map<std::string, std::string>::const_iterator ch_it = params.find("channel");
    std::map<std::string, std::string>::const_iterator nick_it = params.find("nickname");
    
    if (ch_it == params.end() || nick_it == params.end()) {
        std::string error = ":server 461 " + _Nick + " INVITE :Not enough parameters\r\n";
        rcvMsg(error);
        return;
    }
    
    std::string channelName = ch_it->second;
    if (!channelName.empty() && channelName[0] != '#') {
        channelName = "#" + channelName;
    }
    
    Channel* channel = Server::getInstance().IsChannelExist(channelName);
    
    if (!channel) {
        std::string error = ":server 403 " + _Nick + " " + channelName + " :No such channel\r\n";
        rcvMsg(error);
        return;
    }
    

    if (!channel->isUserInChannel(*this)) {
        std::string error = ":server 442 " + _Nick + " " + channelName + " :You're not on that channel\r\n";
        rcvMsg(error);
        return;
    }
    
    Client* targetClient = NULL;
    try {
        targetClient = &Server::getInstance().getUser(nick_it->second);
    } catch (...) {
        std::string error = ":server 401 " + _Nick + " " + nick_it->second + " :No such nick\r\n";
        rcvMsg(error);
        return;
    }
    
    if (channel->isUserInChannel(*targetClient)) {
        std::string error = ":server 443 " + _Nick + " " + nick_it->second + " " + channelName + " :is already on channel\r\n";
        rcvMsg(error);
        return;
    }
    
    if (channel->isInviteOnly() && !channel->isOp(*this)) {
        std::string error = ":server 482 " + _Nick + " " + channelName + " :You're not channel operator\r\n";
        rcvMsg(error);
        return;
    }
    
    try {
        channel->inviteUser(*this, *targetClient);
        
        std::string inviteMsg = ":" + _Nick + "!" + _User + "@" + _hostname + 
                               " INVITE " + nick_it->second + " " + channelName + "\r\n";
        targetClient->rcvMsg(inviteMsg);
        
        std::string confirm = ":server 341 " + _Nick + " " + nick_it->second + " " + channelName + "\r\n";
        rcvMsg(confirm);
        
        std::cout << _Nick << " invited " << nick_it->second << " to " << channelName << std::endl;
        
    } catch (std::exception& e) {
        std::string error = ":server 482 " + _Nick + " " + channelName + " :" + e.what() + "\r\n";
        rcvMsg(error);
    }
}

void Client::handleTopicCommand(const std::map<std::string, std::string>& params) {
    std::map<std::string, std::string>::const_iterator ch_it = params.find("channel");
    
    if (ch_it == params.end()) {
        std::string error = ":server 461 " + _Nick + " TOPIC :Not enough parameters\r\n";
        rcvMsg(error);
        return;
    }
    
    std::string channelName = ch_it->second;
    if (!channelName.empty() && channelName[0] != '#') {
        channelName = "#" + channelName;
    }
    
    Channel* channel = Server::getInstance().IsChannelExist(channelName);
    
    if (!channel) {
        std::string error = ":server 403 " + _Nick + " " + channelName + " :No such channel\r\n";
        rcvMsg(error);
        return;
    }
    
    if (!channel->isUserInChannel(*this)) {
        std::string error = ":server 442 " + _Nick + " " + channelName + " :You're not on that channel\r\n";
        rcvMsg(error);
        return;
    }
    
    std::map<std::string, std::string>::const_iterator topic_it = params.find("topic");
    
    if (topic_it != params.end()) {
        if (channel->isTopicRestricted() && !channel->isOp(*this)) {
            std::string error = ":server 482 " + _Nick + " " + channelName + " :You're not channel operator\r\n";
            rcvMsg(error);
            return;
        }
        
        try {
            channel->setTopic(*this, topic_it->second);
            std::cout << _Nick << " set topic for " << channelName << ": " << topic_it->second << std::endl;
        } catch (std::exception& e) {
            std::string error = ":server 482 " + _Nick + " " + channelName + " :You're not channel operator\r\n";
            rcvMsg(error);
        }
    } else {
        const std::string& topic = channel->getTopic();
        if (topic.empty()) {
            std::string reply = ":server 331 " + _Nick + " " + channelName + " :No topic is set\r\n";
            rcvMsg(reply);
        } else {
            std::string reply = ":server 332 " + _Nick + " " + channelName + " :" + topic + "\r\n";
            rcvMsg(reply);
        }
    }
}

void Client::handleModeCommand(const std::map<std::string, std::string>& params) {
    std::map<std::string, std::string>::const_iterator target_it = params.find("target_type");
    
    if (target_it != params.end() && target_it->second == "channel") {
        std::map<std::string, std::string>::const_iterator ch_it = params.find("channel");
        std::map<std::string, std::string>::const_iterator mode_it = params.find("mode");
        
        if (ch_it == params.end()) {
            std::string error = ":server 461 " + _Nick + " MODE :Not enough parameters\r\n";
            rcvMsg(error);
            return;
        }
        
        std::string channelName = ch_it->second;
        if (!channelName.empty() && channelName[0] != '#') {
            channelName = "#" + channelName;
        }
        
        Channel* channel = Server::getInstance().IsChannelExist(channelName);
        
        if (!channel) {
            std::string error = ":server 403 " + _Nick + " " + channelName + " :No such channel\r\n";
            rcvMsg(error);
            return;
        }
        
        if (!channel->isUserInChannel(*this)) {
            std::string error = ":server 442 " + _Nick + " " + channelName + " :You're not on that channel\r\n";
            rcvMsg(error);
            return;
        }
        
        if (mode_it != params.end()) {
            if (!channel->isOp(*this)) {
                std::string error = ":server 482 " + _Nick + " " + channelName + " :You're not channel operator\r\n";
                rcvMsg(error);
                return;
            }
            
            try {
                std::string param = "";
                std::map<std::string, std::string>::const_iterator param_it = params.find("mode_param");
                if (param_it != params.end()) {
                    param = param_it->second;
                }
                
                channel->setMode(*this, mode_it->second, param);
                std::cout << _Nick << " set mode " << mode_it->second << " on " << channelName << std::endl;
                
            } catch (std::exception& e) {
                std::string error = ":server 501 " + _Nick + " :Unknown MODE flag\r\n";
                rcvMsg(error);
            }
        } else {
            std::string reply = ":server 324 " + _Nick + " " + channelName + " +nt\r\n";
            rcvMsg(reply);
        }
    }
}

void Client::handlePartCommand(const std::map<std::string, std::string>& params) {
    std::map<std::string, std::string>::const_iterator ch_it = params.find("channel");
    
    if (ch_it == params.end()) {
        std::string error = ":server 461 " + _Nick + " PART :Not enough parameters\r\n";
        rcvMsg(error);
        return;
    }
    
    std::string channelName = ch_it->second;
    // Normalize channel name
    if (!channelName.empty() && channelName[0] != '#') {
        channelName = "#" + channelName;
    }
    
    Channel* channel = Server::getInstance().IsChannelExist(channelName);
    
    if (!channel) {
        std::string error = ":server 403 " + _Nick + " " + channelName + " :No such channel\r\n";
        rcvMsg(error);
        return;
    }
    
    if (!channel->isUserInChannel(*this)) {
        std::string error = ":server 442 " + _Nick + " " + channelName + " :You're not on that channel\r\n";
        rcvMsg(error);
        return;
    }
    
    std::string partMsg = "Leaving";
    std::map<std::string, std::string>::const_iterator msg_it = params.find("message");
    if (msg_it != params.end()) {
        partMsg = msg_it->second;
    }
    
    std::string broadcastMsg = ":" + _Nick + "!" + _User + "@" + _hostname + 
                              " PART " + channelName + " :" + partMsg + "\r\n";
    
    channel->broadcastMessage(broadcastMsg);
    
    channel->removeUser(*this);
    
    for (std::vector<Channel*>::iterator it = _subscribed2Channel.begin();
         it != _subscribed2Channel.end(); ++it) {
        if (*it == channel) {
            _subscribed2Channel.erase(it);
            break;
        }
    }
    
    std::cout << _Nick << " left channel " + channelName << std::endl;
}