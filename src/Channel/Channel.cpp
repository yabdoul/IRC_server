#include  "Channel.hpp"
#include "channelCommand.hpp"  
#include <algorithm>  
#include "Client.hpp"
#include <vector>
#include <cstdlib>  
enum Roles {
    OP,
    INV,
    MEMB
};

Channel::Channel(){  
      _invitOnly = false;
      _topicRestricted = false;
      _userLimit = 0;
}  
Channel::~Channel(){}   
// Channel::Channel(Channel &copy){ _inviteList =  copy._inviteList ;}    
Channel::Channel(std::string channelName  ,  Client & owner):_channelName(channelName)
{ 
      _inviteList[owner] = OP;
      _invitOnly = false;
      _topicRestricted = false;
      _userLimit = 0;
}


void Channel::ExecuteCommand(Command & cmd  ,  Client client   ,  std::map<std::string  , std::string>params  )      
{       
      if(dynamic_cast<ChannelCommand *> (&cmd) )  
      { 
            ChannelCommand * tmp =  dynamic_cast<ChannelCommand *>  (&cmd) ;    
            tmp->exeChannel(client ,  *this , params) ;   
      }   

}  ;   

bool Channel::isOp(Client & sender )  {
     return((_inviteList[sender] ==OP)?(true):(false)) ;}  

void Channel::inviteUser(Client &sender ,  Client  &target )    
{    
      (void)sender;
      if(_inviteList.count(target) != 0 )  
            throw std::runtime_error("User already in channel or invite list") ;    
      
      // (invited but not yet joined)
      _inviteList[target] =  INV ;      
}  ;   
 
void Channel::lockChannel(Client &sender)  
{ 
      _invitOnly = ((isOp(sender))?true:false)  ;  
}
void Channel::enterChannel(Client &cl  ) 
{    
      if(!_invitOnly || _inviteList.count(cl) > 0)   
      {
          _inviteList[cl] = MEMB;
          return;
      }
      
      throw std::runtime_error("Cannot join channel (+i)");   
}

void Channel::kickUser(Client &sender, Client &target, const std::string& reason) {
    if (!isOp(sender)) {
        throw std::runtime_error("You're not channel operator");
    }
    
    if (_inviteList.find(target) == _inviteList.end()) {
        throw std::runtime_error("They aren't on that channel");
    }
    
    _inviteList.erase(target);
    
    std::string kickMsg = ":" + sender.getNickname() + " KICK #" + _channelName + " " + 
                         target.getNickname() + " :" + reason + "\r\n";
    broadcastMessage(kickMsg);
}

void Channel::removeUser(Client &client) {
    _inviteList.erase(client);
}

void Channel::setTopic(Client &sender, const std::string& newTopic) {
    if (_topicRestricted && !isOp(sender)) {
        throw std::runtime_error("You're not channel operator");
    }
    
    _topic = newTopic;
    
    std::string topicMsg = ":" + sender.getNickname() + " TOPIC #" + _channelName + 
                          " :" + newTopic + "\r\n";
    broadcastMessage(topicMsg);
}

const std::string& Channel::getTopic() const {
    return _topic;
}

void Channel::setMode(Client &sender, const std::string& mode, const std::string& param) {
    if (!isOp(sender)) {
        throw std::runtime_error("You're not channel operator");
    }
    
    bool adding = (mode[0] == '+');
    char modeChar = mode[1];
    
    switch (modeChar) {
        case 'i': // Invite-only
            _invitOnly = adding;
            break;
        case 't': // Topic restricted
            _topicRestricted = adding;
            break;
        case 'k': // Channel key (password)
            if (adding) {
                _key = param;
            } else {
                _key.clear();
            }
            break;
        case 'o': // Give/take operator privilege
            if (!param.empty()) {
                for (std::map<Client, int>::iterator it = _inviteList.begin(); 
                     it != _inviteList.end(); ++it) {
                    if (it->first.getNickname() == param) {
                        it->second = adding ? OP : MEMB;
                        break;
                    }
                }
            }
            break;
        case 'l': // User limit
            if (adding) {
                _userLimit = std::atoi(param.c_str());
            } else {
                _userLimit = 0;
            }
            break;
    }
    
    std::string modeMsg = ":" + sender.getNickname() + " MODE #" + _channelName + " " + mode;
    if (!param.empty()) {
        modeMsg += " " + param;
    }
    modeMsg += "\r\n";
    broadcastMessage(modeMsg);
}

void Channel::broadcastMessage(const std::string& message, Client* exclude) {
    for (std::map<Client, int>::iterator it = _inviteList.begin(); 
         it != _inviteList.end(); ++it) {
        if (exclude == NULL || !(it->first == *exclude)) {
            std::string msg = message;
            it->first.rcvMsg(msg);
        }
    }
}

bool Channel::isUserInChannel(const Client& user) const {
    return _inviteList.find(const_cast<Client&>(user)) != _inviteList.end();
}

std::vector<Client*> Channel::getChannelMembers() const {
    std::vector<Client*> members;
    for (std::map<Client, int>::const_iterator it = _inviteList.begin(); 
         it != _inviteList.end(); ++it) {
        members.push_back(const_cast<Client*>(&it->first));
    }
    return members;
}

const std::string& Channel::getName() const {
    return _channelName;
}

bool Channel::isInviteOnly() const {
    return _invitOnly;
}

bool Channel::isTopicRestricted() const {
    return _topicRestricted;
}

bool Channel::isUserInvited(const Client& user) const {
    std::map<Client, int>::const_iterator it = _inviteList.find(const_cast<Client&>(user));
    return (it != _inviteList.end() && it->second == INV);
}