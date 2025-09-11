#include <map>  
#include  "Client.hpp" 
#include "Command.hpp"    
#include <algorithm>  
#include "Ichannel.hpp"  
#include <algorithm>  
#include <exception>        

class Channel :  public Ichannel {  
        private :  
                 std::map<Client  , std::string   >    _inviteList ;    
                 std::vector<Client> _online ;         
                 bool _invitOnly ;   
        public :   
                Channel() ;   
                ~Channel();         
                Channel& operator=(Channel &c )    ;   
                void ExecuteCommand(Command  *  cmd  , Client Client  )  ;    
                void handleKick(Client& sender,  std::string& targetNick, const std::string& reason)  ;   
                void handleJoin(Client& sender )  ;        
                void rcvMsg(std::string  &msg  )  ; 
                void lockChannel(Client & sender ) ;      
                void handleInvite(Client& sender, Client& target)  ;   
                // void handleTopic(Client& sender, const std::string& newTopic = "");
                // void handleMode(Client& sender , const std::string& mode, const std::string& param = "") ; 
                // void IsSubscribed(Client &C  )  const    ;   
} ;   